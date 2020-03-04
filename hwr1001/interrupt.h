#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>

extern int16_t OverflowCount;              //编码器计数溢出 计数器
extern uint8_t led_display_cnt_flag;  
//extern uint16_t time3_count;
extern uint16_t iTaskTimCount;
extern bool g_bEnUartDebugSend;

uint8_t Uart_Receive_Interrupt_Switch (UART_HandleTypeDef *huart, uint8_t *uart_receive_data);
#endif
