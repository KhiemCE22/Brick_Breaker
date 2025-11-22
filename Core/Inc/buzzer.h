/*
 * buzzer.h
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

/* Includes */
#include <stdint.h>

/* Functions */
void buzzer_init();
void buzzer_set_volume(uint8_t duty_cycle);

void buzzer_start_fade(int start_volume);

/* Gọi trong interrupt TIMx mỗi 1 ms */
void buzzer_tick_ms(void);

void buzzer_set_frequency(uint32_t freq);

/* Hàm phát 1 nốt */
void buzzer_play_note(uint32_t freq, uint16_t duration_ms);

/* Hàm nhạc intro */
void buzzer_intro(void);

#endif /* INC_BUZZER_H_ */
