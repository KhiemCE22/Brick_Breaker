/*
 * button.h
 */

//#ifndef INC_BUTTON_H_
//#define INC_BUTTON_H_
//
///* Includes */
//#include <stdint.h>
//
///* Variables */
//extern uint16_t button_count[16];
//
///* Functions */
//extern void button_init();
//extern void button_scan();
//
//#endif /* INC_BUTTON_H_ */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

void button_scan();             // gọi mỗi 50 ms
uint8_t button_clicked(uint8_t id); // trả về 1 nếu nút id nhấn 1 lần
void buzzer_intro_start(void);
void buzzer_intro_tick(void) ;

#endif /* INC_BUTTON_H_ */
