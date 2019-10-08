/*
 * uart_test.c
 *
 *  Created on: 2019. 9. 2.
 *      Author: bitcamp
 */


#include "uart_test.h"

char  tx_test[100];
char ch, ch2, rx_Test[100];
int index1=0;
int split_index=0;
int char_flag=0;

void UART6_Test(void)
{
//PE5_High();
//UART6_485_EN_High();
//	printf("uart start!\n");
    /* Infinite loop */
	while(CRZ_FALSE == Uart_6_Queue_Comm_Is_Empty())
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
#if 0
		ch = Uart_6_Queue_Comm_DeQueue();
		sprintf(rx_Test,"%c",ch);
		HAL_UART_Transmit(&huart1, (uint8_t *)rx_Test, strlen(rx_Test), 0xFFFF);
#endif

#if 0
		rx_Test[index1++]= Uart_6_Queue_Comm_DeQueue();
		//MANGO_DBG_BLUE("Rx: %c (0x%02X)\n", ch, ch);
		//printf("%c",th);
		if(rx_Test[index1 - 1] == ',')
		{
			rx_Test[index1 - 1] = '\0';
			band_info[split_index++] = atoi(rx_Test);
			index1 = 0;
		}

		if(rx_Test[index1 - 1] == '\n')
		{
			rx_Test[index1 -1] = '\0';
			band_info[split_index++] = atoi(rx_Test);
			split_index = 0;
			//sprintf(rx_Test, "flag:%f, roll:%f, pitch:%f\n\r",gyro_info[FLAG],gyro_info[ROLL],gyro_info[PITCH]);
			//printf("%d , %d , %d \n",band_info[FLAG],band_info[ROLL],band_info[PITCH]);
			//HAL_UART_Transmit(&huart1, (uint8_t *)rx_Test, strlen(rx_Test), 0xFFFF); //이 부분에서 필요한 곳으로 데이터를 보내면 됩니다.
			index1 = 0;
		}


		//sprintf(rx_Test,"%c",th);
		//sprintf(rx_Test,"\n");

		//printf("%c",rx_Test);
#endif
#if 1
		char cha = Uart_6_Queue_Comm_DeQueue();
		if(char_flag){
			rx_Test[index1++] = cha;
			if(rx_Test[index1 - 1] == ',')
			{
				rx_Test[index1 - 1] = '\0';
				band_info[split_index++] = atoi(rx_Test);
				index1 = 0;
			}
			if(rx_Test[index1 - 1] == '\n')
			{
				rx_Test[index1 -1] = '\0';
				band_info[split_index++] = atoi(rx_Test);
				split_index = 0;
				//printf("%d , %d , %d \n",band_info[FLAG],band_info[ROLL],band_info[PITCH]);
				index1 = 0;
				char_flag = 0;
			}
		}
		if(cha == '[')
			char_flag = 1;
#endif
	}

//	if(CRZ_FALSE == UartQueue_Serial_Is_Empty())
//	{
		//print_que();
//		ch2=UartQueue_Serial_DeQueue();
//		sprintf(tx_test,"%c",ch2);
		//sprintf(tx_test, "Test chars %d\r\n", count);
//		HAL_UART_Transmit(&huart6, (uint8_t *)tx_test, strlen(tx_test), 0xFFFF);
//		HAL_UART_Transmit(&huart6, (uint8_t *)"\n\r", strlen("\n\r"), 0xFFFF);
//	}

}
