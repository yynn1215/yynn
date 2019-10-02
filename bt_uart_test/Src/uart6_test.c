
#include "uart6_test.h"


static CRZ_bool g_print_menu_control_flag = CRZ_TRUE;

static void print_menu_command(void)
{
    if(CRZ_TRUE == g_print_menu_control_flag)
    {
        printf("\n---------------------\n");
        printf("Press menu key\n");
        printf("---------------------\n");
        printf("1> RS232 Test\n");
        printf("2> RS485 Test\n");
        printf("---------------------\n");
        printf("x> exit\n");
        printf("---------------------\n");
    }

    g_print_menu_control_flag = CRZ_FALSE;
}

static void PE5_High(void)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
}

static void PE5_Low(void)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
}


void UART6_Test(void)
{
    printf("UART6Test\n");
    char ch,rh, tx_test[100],rx_test[100];
    int count = 0;
    PE5_High();
    //MANGO_DBG_DEFAULT_BLUE;

    //UART_6_Com_Initialization();
    //PE5_Init();
    //UART6_485_EN_Init();

//PE5_High();
//UART6_485_EN_High();

    /* Infinite loop */
    while (1)
    {

    	print_menu_command();

    	if(CRZ_TRUE == UartQueue_Serial_Is_Empty())
    	{
    		continue;
    	}

    	ch = UartQueue_Serial_DeQueue();
  	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
    	if(0 != ch)
    	{
    		g_print_menu_control_flag = CRZ_TRUE;

    		printf("%c is selected\n", ch);

    		switch(ch)
    		{
    		case '1':
    			PE5_High();

    			/* Infinite loop */
    			while (1)
    			{
    				count ++;

    				while(CRZ_FALSE == Uart_6_Queue_Comm_Is_Empty())
    				{
    					//printf("Uart_6_test\n");
    					//print_que();
    					rh = Uart_6_Queue_Comm_DeQueue();
    					sprintf(rx_test,"%c",rh);
    					//MANGO_DBG_BLUE("Rx: %c (0x%02X)\n", ch, ch);
    					HAL_UART_Transmit(&huart1, (uint8_t *)rx_test, strlen(rx_test), 0xFFFF);
    				}
    				//HAL_Delay(1000);
    				if(CRZ_FALSE == UartQueue_Serial_Is_Empty())
    				{
    					//printf("Uart_1_test\n");
    					print_que();
    					ch = UartQueue_Serial_DeQueue();
    					sprintf(tx_test, "%c", ch);
    					//sprintf(tx_test, "Test chars %d\r\n", count);
    					HAL_UART_Transmit(&huart6, (uint8_t *)tx_test, strlen(tx_test), 0xFFFF); // 1. converter 문제일 수 있음
    				}
    			}
    			break;

    		case '2':
    			PE5_Low();
    			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);

    			/* Infinite loop */
    			while (1)
    			{
    				while(CRZ_FALSE == Uart_6_Queue_Comm_Is_Empty())
    				{
    					//PE5_Low();
    					//UART6_485_EN_Low();
    					ch = Uart_6_Queue_Comm_DeQueue();
    					printf("%c", ch);
    				}

    				if(CRZ_FALSE == UartQueue_Serial_Is_Empty())
    				{
    					ch = UartQueue_Serial_DeQueue();

    					sprintf(tx_test, "%c", ch);
    					//UART6_485_EN_High();
    					//HAL_UART_Transmit(&g_Uart_6_Com_Handle, (uint8_t *)tx_test, strlen(tx_test), 0xFFFF);
    					//UART6_485_EN_Low();
    				}
    			}
    			break;

    		case 'x':
    			return;
    		}
    	}
    }
}
