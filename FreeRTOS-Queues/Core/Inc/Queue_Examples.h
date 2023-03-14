
// simple queue Example

#ifndef QUEUE_EXAMPLES_H
#define QUEUE_EXAMPLES_H

#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
//#include "cmsis_os.h"

extern  QueueHandle_t xQueue1;
extern UART_HandleTypeDef huart3;

#define UART_MSG_LEN 10
#define QUEUE_BUFFER_DEPTH 5

void rcv_task (void * params);
void send_task (void * params);
void msg_parser (void * params);



typedef struct queue_buffer
{
	char opcode[2];
	char data[8];
	uint16_t is_busy;
}Queue_buffer;





#endif /* QUEUE_EXAMPLES_H */
