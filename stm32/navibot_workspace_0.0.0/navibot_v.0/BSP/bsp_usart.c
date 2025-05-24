/*
 * bsp_usart.c
 *
 *  Created on: Apr 30, 2025
 *      Author: Carlos
 */


#include "bsp_usart.h"
#include "main.h"
#include "bsp.h"
#include "app_protocol.h"

#include "string.h"

extern UART_HandleTypeDef huart1;

#define ENABLE_UART_DMA


volatile uint8_t uart1_dma_busy = 0;



#define UART_RX_BUFFER_SIZE 64
uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
uint8_t user_rx_buffer[UART_RX_BUFFER_SIZE];




// Initialize USART1

void USART1_Init(void)
{

	HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_RX_BUFFER_SIZE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

}

void USART1_Send_U8(uint8_t ch)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
}

// The serial port sends a string of data
void USART1_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length)
{
	#ifdef ENABLE_UART_DMA
	//if (!uart1_dma_busy)
	//	    {
	//	        uart1_dma_busy = 1;
		        HAL_UART_Transmit_DMA(&huart1, BufferPtr, Length);
	//	    }

	#else
	while (Length--)
	{

		USART1_Send_U8(*BufferPtr);
	}
	#endif
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    //if (huart->Instance == USART1) {
    //    uart1_dma_busy = 0;
    //}
}


// Code for printf

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	// Place your implementation of fputc here
	// e.g. write a character to the EVAL_COM1 and Loop until the end of transmission
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

