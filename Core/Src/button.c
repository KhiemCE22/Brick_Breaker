/*
 * button.c
 */

#include "button.h"
#include "spi.h"
#include "gpio.h"

/* ================= CONFIG ================= */

#define BUTTON_NUM          16
#define DEBOUNCE_LEVEL      3   // yêu cầu 3 mẫu ổn định (50ms x 3 = 150ms)

/* ================ VARIABLES ================ */

typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED
} ButtonState;

static ButtonState btn_state[BUTTON_NUM] = { BTN_RELEASED };
static uint8_t btn_event[BUTTON_NUM] = { 0 };
static uint8_t debounce_cnt[BUTTON_NUM] = { 0 };

static uint16_t button_spi_buffer = 0x0000;

/* ============== INTERNAL FUNCTIONS ============== */

static void button_process_bit(uint8_t id, uint8_t raw_state)
{
    if (raw_state == 0)  // Nút đang nhấn (bit = 0)
    {
        if (debounce_cnt[id] < DEBOUNCE_LEVEL)
        {
            debounce_cnt[id]++;
        }
        else
        {
            if (btn_state[id] == BTN_RELEASED)
            {
                btn_state[id] = BTN_PRESSED;
                btn_event[id] = 1;        // sinh event nhấn 1 lần
            }
        }
    }
    else // raw_state = 1 → nút thả
    {
        if (debounce_cnt[id] > 0)
        {
            debounce_cnt[id]--;
        }
        else
        {
            btn_state[id] = BTN_RELEASED;
        }
    }
}

/* ============== PUBLIC FUNCTIONS ============== */

/**
 * @brief  Scan matrix buttons (gọi mỗi 50ms)
 */
void button_scan()
{
    // Chốt dữ liệu từ shift-register
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 0);
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);

    // Đọc SPI (16 bit)
    HAL_SPI_Receive(&hspi1, (uint8_t *)&button_spi_buffer, 2, 10);

    uint16_t mask = 0x8000;

    for (int i = 0; i < 16; i++)
    {
        int index;

        // Mapping thứ tự bit -> thứ tự nút thật
        if (i >= 0 && i <= 3)
            index = i + 4;
        else if (i >= 4 && i <= 7)
            index = 7 - i;
        else if (i >= 8 && i <= 11)
            index = i + 4;
        else
            index = 23 - i;

        uint8_t raw = (button_spi_buffer & mask) ? 1 : 0;

        button_process_bit(index, raw);

        mask >>= 1;
    }
}

/**
 * @brief  Kiểm tra nút đã nhấn 1 lần hay chưa (event click)
 * @param  id: số nút (0..15)
 * @retval 1 nếu có event click, 0 nếu không
 */
uint8_t button_clicked(uint8_t id)
{
    if (btn_event[id])
    {
        btn_event[id] = 0;  // reset event ngay sau khi đọc
        return 1;
    }
    return 0;
}
