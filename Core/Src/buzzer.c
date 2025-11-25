/*
 * buzzer.c
 */

/* Includes */
#include "buzzer.h"
#include "tim.h"
#include "stm32f4xx_hal.h"

/* Variables */
uint8_t buzzerduty_cycle = 0;

/* Lưu trạng thái ban đầu trước khi chơi intro */
static uint32_t buzzer_freq_before_intro = 0;
static uint8_t  buzzer_vol_before_intro = 0;

/* Các biến dùng cho fade volume */
volatile static int buzzer_current = 0;
volatile static int buzzer_target = 0;
volatile static int buzzer_fading = 0;

/* ==================== Functions ==================== */

void buzzer_init(void) {
    HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
}

void buzzer_set_volume(uint8_t duty_cycle) {
    buzzerduty_cycle = duty_cycle;
    __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, buzzerduty_cycle);
}

void buzzer_start_fade(int start_volume)
{
    buzzer_current = start_volume;
    buzzer_target = 0;
    buzzer_fading = 1;
    buzzer_set_volume((uint8_t)buzzer_current);
}

/* Gọi trong interrupt TIMx mỗi 1 ms */
void buzzer_tick_ms(void)
{
    if (!buzzer_fading) return;
    if (buzzer_current > buzzer_target) {
        buzzer_current--;
        buzzer_set_volume((uint8_t)buzzer_current);
    } else {
        buzzer_fading = 0;
    }
}

void buzzer_set_frequency(uint32_t freq)
{
    if (freq == 0) {
        __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 0);
        return;
    }

    uint32_t f_timer = 84000000 / (htim13.Init.Prescaler + 1); // 100 kHz
    uint32_t period = f_timer / freq - 1;

    htim13.Instance->ARR = period;
    htim13.Instance->CCR1 = period / 2;    // duty 50%
    __HAL_TIM_SET_COUNTER(&htim13, 0);
}

/* Hàm phát 1 nốt */
void buzzer_play_note(uint32_t freq, uint16_t duration_ms)
{
    buzzer_set_frequency(freq);
    buzzer_set_volume(50);
    HAL_Delay(duration_ms);
}

/* Hàm nhạc intro */
void buzzer_intro(void)
{
    /* Lưu trạng thái hiện tại trước khi chơi */
    buzzer_freq_before_intro = (htim13.Instance->ARR + 1);
//    buzzer_freq_before_intro = buzzer_freq_before_intro ? 100000 / buzzer_freq_before_intro : 0;
    buzzer_vol_before_intro  = buzzerduty_cycle;

    /* Giai điệu intro 8-bit */
    buzzer_play_note(523, 180); HAL_Delay(30);  // C5
    buzzer_play_note(659, 180); HAL_Delay(30);  // E5
    buzzer_play_note(784, 180); HAL_Delay(30);  // G5
    buzzer_play_note(1046, 300); HAL_Delay(60); // C6

    buzzer_set_frequency(0); HAL_Delay(80);      // nghỉ

    buzzer_play_note(784, 200); HAL_Delay(30);  // G5
    buzzer_play_note(1046, 350);                // C6               // C6

    /* Khôi phục trạng thái ban đầu */
    buzzer_set_frequency(1000);
    buzzer_set_volume(buzzer_vol_before_intro);
}

typedef struct {
    uint32_t notes[10];     // tần số nốt
    uint16_t durations[10]; // thời gian nốt (ms)
    uint8_t current_note;   // nốt đang phát
    uint16_t tick_count;    // đếm ms
    uint8_t playing;        // đang chơi intro
} BuzzerIntro;

BuzzerIntro intro = {0};

void buzzer_intro_start(void)
{
    intro.notes[0] = 523; intro.durations[0] = 280;
    intro.notes[1] = 659; intro.durations[1] = 280;
    intro.notes[2] = 784; intro.durations[2] = 280;
    intro.notes[3] = 1046; intro.durations[3] = 500;
    intro.current_note = 0;
    intro.tick_count = 0;
    intro.playing = 1;
}

void buzzer_intro_tick(void) // gọi trong game loop hoặc timer 1ms
{
    if (!intro.playing) return;

    if (intro.tick_count == 0) {
        // phát nốt hiện tại
        buzzer_set_frequency(intro.notes[intro.current_note]);
        buzzer_set_volume(50);
    }

    intro.tick_count++;
    if (intro.tick_count >= intro.durations[intro.current_note]) {
        intro.current_note++;
        intro.tick_count = 0;

        if (intro.current_note >= sizeof(intro.notes)/sizeof(intro.notes[0])) {
            intro.playing = 0;
            buzzer_set_frequency(0); // tắt buzzer
        }
    }
}
