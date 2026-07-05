/*
 * Uart.h
 *
 *  Created on: 2025年3月18日
 *      Author: 19131
 */

#include "zf_common_headfile.h"

#ifndef CODE_UART_UART_H_
#define CODE_UART_UART_H_

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // 默认 UART_0
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默认 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // 默认 UART0_TX_P14_0
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // 默认 UART0_RX_P14_1

extern float data_uart;

extern uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
extern uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

extern uint8  get_data;                                                            // 接收数据变量
extern uint32 fifo_data_count;                                                     // fifo 数据个数

extern fifo_struct uart_data_fifo;

void Uart_Init(void);
void Uart_PID(void);

#endif /* CODE_UART_UART_H_ */
