#ifndef UART6_TEST_H_
#define UART6_TEST_H_

/* includes */

#include "stm32f4xx_hal.h"
#include "uart_queue.h"
#include <stdio.h>
#include <string.h>


/* defines */

/* Definition for USARTx clock resources */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
/* functions */
static void print_menu_command(void);
void UART6_Test(void);

#endif /* UART6_TEST_H_ */
