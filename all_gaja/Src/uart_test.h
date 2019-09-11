/*
 * uart_test.h
 *
 *  Created on: 2019. 9. 2.
 *      Author: bitcamp
 */

#ifndef UART_TEST_H_
#define UART_TEST_H_

#include "usart.h"
#include "uart_queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FLAG 	0
#define ROLL	1
#define PITCH	2
int band_info[3];
extern int index1;
extern int split_index;


extern char  tx_test[100];
extern char ch, ch2, rx_Test[100];

void UART6_Test(void);


#endif /* UART_TEST_H_ */
