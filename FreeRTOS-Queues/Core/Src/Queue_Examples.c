/*
 * Queue_Examples.c
 *
 *  Created on: Jan 30, 2023
 *      Author: ELIRANBM
 */

#include "Queue_Examples.h"
#include "string.h"


uint8_t uart3_income_buffer[10]= {0};
uint32_t  address_input_buffer;
uint8_t parsing_buffer[10]= {0};


extern struct netif gnetif;

 static Queue_buffer queue_buffers_array[QUEUE_BUFFER_DEPTH];

void rcv_task (void * params)
{

	// init queue buffers
	for(int i = 0; i< QUEUE_BUFFER_DEPTH; i++ )
		{
			queue_buffers_array[i].is_busy = 0;
		}

	while(1)
	{


		// wait for uart incoming
		HAL_StatusTypeDef status = HAL_UART_Receive (&huart3, uart3_income_buffer, UART_MSG_LEN,0xFFFFFFF0);
		if(status == HAL_OK)
		{

			// To Do possible checkSum test

			for(int i = 0; i< QUEUE_BUFFER_DEPTH; i++ )
			{
				if(!queue_buffers_array[i].is_busy)
				{
					// mutex
					memcpy(&queue_buffers_array[i],uart3_income_buffer,sizeof(uart3_income_buffer));
					queue_buffers_array[i].is_busy = 1;
					// end mutex

					//sending to the queue
					int x = &queue_buffers_array[i];
					xQueueSend( xQueue1, &x, portMAX_DELAY );

					break;
				}
			}

		}

		else
		{


			if(status == HAL_TIMEOUT)
			{
			// send uart msg for timeout or error
						HAL_UART_Transmit(&huart3, "rcv_task got timeout in UART\r\n", strlen("rcv_task got timeout in UART\r\n"), 1000);
			}


				else
				{
				// send uart msg for timeout or error
							HAL_UART_Transmit(&huart3, "rcv_task ERROR!!! in UART\r\n", strlen("rcv_task ERROR!!! in UART\r\n"), 1000);
				}
		}



	}
}
void send_task (void * params)
{
	while(1)
		{

		}
}



void msg_parser (void * params)
{
	HAL_StatusTypeDef status;
	int16_t opCode = 0;
	char tmp[2];
	char msg[20] = "toggle green led\n";
	char command[15] = {'\0'};
	while(1)
		{


		 xQueueReceive( xQueue1, &address_input_buffer, portMAX_DELAY);
		 //mutex

		 // here i need to parse & do an action

		 memcpy(&tmp,(char *)address_input_buffer,2);
		 memcpy(&command,(char *)address_input_buffer+2,8);
		 opCode = atoi(tmp);
		 //end mutex

		 switch (opCode)
		 {
		 // green LED
		 case 1:
			 HAL_GPIO_TogglePin(Green_LED_GPIO_Port, Green_LED_Pin);
			 status =  HAL_UART_Transmit(&huart3,"toggle blue led\n" , strlen("toggle blue led\n"), 1000);
			 break;

		// Blue LED
		 case 2:
			 HAL_GPIO_TogglePin(Blue_LED_GPIO_Port, Blue_LED_Pin);
			 status =  HAL_UART_Transmit(&huart3,msg , strlen(msg), 1000);
			 break;

		//return echo code
		 case 3:
			 memcpy(command +8 ,"\n\0", 2);
			 HAL_UART_Transmit(&huart3, command, strlen(command), 1000);
			 memset(command,0,strlen(command));
			 break;

		 default:
			 break;
		 }

		 ((Queue_buffer *)address_input_buffer)->is_busy = 0;


		}
}


