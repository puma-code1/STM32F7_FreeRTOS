
// simple queue Example

#ifndef QUEUE_EXAMPLES_H
#define QUEUE_EXAMPLES_H

#include "FreeRTOS.h"
#include "queue.h"


extern  QueueHandle_t xQueue1;

void rcv_task (void * params);
void send_task (void * params);










#endif /* QUEUE_EXAMPLES_H */
