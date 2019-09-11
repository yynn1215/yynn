/*
 * uart_queue.h
 *
 *  Created on: 2019. 9. 2.
 *      Author: bitcamp
 */

#ifndef UART_QUEUE_H_
#define UART_QUEUE_H_

#include "usart.h"

#define  UART_QUEUE_BUFFER_SIZE  1000
#define  EVENT_QUEUE_BUFFER_SIZE  200

void print_que();

// UART Queue for Serial

void UartQueue_Serial_Initialize(void);
CRZ_bool UartQueue_Serial_Is_Empty(void);
void UartQueue_Serial_EnQueue(uint16_t data);
uint16_t UartQueue_Serial_DeQueue(void);

// UART 6 Queue for Comm

void Uart_6_Queue_Comm_Initialize(void);
CRZ_bool Uart_6_Queue_Comm_Is_Empty(void);
void Uart_6_Queue_Comm_EnQueue(uint16_t data);
uint16_t Uart_6_Queue_Comm_DeQueue(void);

// EVENT Queue for Comm
typedef enum
{
    CRZ_EVENT_NONE = 0,

    /* Work Events */

    CRZ_EVENT_WORK_USER_KEY_1_PRESS          = 0x1,
    CRZ_EVENT_WORK_USER_KEY_1_RELEASE        = 0x2,
    CRZ_EVENT_WORK_USER_KEY_2_PRESS          = 0x3,
    CRZ_EVENT_WORK_USER_KEY_2_RELEASE        = 0x4,

    /* Error Events */

#if 0
    CRZ_EVENT_ERROR_...         = 0xE0,
    CRZ_EVENT_ERROR_...         = 0xE1,
    CRZ_EVENT_ERROR_...         = 0xE4,
#endif

    CRZ_EVENT_MAX  = 0xFFFFFFFF
}
EventQueue_EventID_E;

typedef struct
{
    EventQueue_EventID_E  event_id;
    uint32_t              event_data;
}
EventQueue_Data_S;

/* Exported functions */

// EVENT Queue for Comm

void EventQueue_Initialize(void);
CRZ_bool EventQueue_Is_Empty(void);
void EventQueue_EnQueue(EventQueue_EventID_E id, uint32_t data);
EventQueue_Data_S EventQueue_DeQueue(void);

#endif /* UART_QUEUE_H_ */
