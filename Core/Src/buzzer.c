/*
 * buzzer.c
 */

/* Includes */
#include "buzzer.h"
#include "tim.h"
#include "stm32f4xx_hal.h"

/* Variables */
uint8_t buzzerduty_cycle = 0;

/* Functions */
void buzzer_init() {
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
}

void buzzer_set_volume(uint8_t duty_cycle) {
	buzzerduty_cycle = duty_cycle;
	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, buzzerduty_cycle);
}

volatile static int buzzer_current = 0;
volatile static int buzzer_target = 0;
volatile static int buzzer_fading = 0;

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

    uint32_t period = 84000000 / freq - 1; // STM32F4, APB1 84MHz
    htim13.Instance->ARR = period;
    htim13.Instance->CCR1 = period / 2;    // duty 50%
    __HAL_TIM_SET_COUNTER(&htim13, 0);
}

/* Hàm phát 1 nốt */
void buzzer_play_note(uint32_t freq, uint16_t duration_ms)
{
    buzzer_set_frequency(freq);
    buzzer_set_volume(50);
    HAL_Delay(duration_ms);   // nhạc intro đơn giản, có thể block
}

/* Hàm nhạc intro */
void buzzer_intro(void)
{
    // Giai điệu kiểu 8-bit
    buzzer_play_note(523, 1200);   // C5
    buzzer_play_note(659, 1200);   // E5
    buzzer_play_note(784, 1200);   // G5
    buzzer_play_note(1046, 1500);  // C6

    buzzer_play_note(988, 1000);   // B5
    buzzer_play_note(1046, 2000);  // C6 - kết thúc

    buzzer_set_volume(0);         // tắt âm
}
