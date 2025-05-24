/*
 * bsp_usart.h
 *
 *  Created on: Apr 30, 2025
 *      Author: Carlos
 */

#ifndef BSP_USART_H_
#define BSP_USART_H_

#include "main.h"
#include "stdint.h"

/* Function Declarations ------------------------------------------------------------------*/
void USART1_Init(void);
void USART1_Send_U8(uint8_t ch);
void USART1_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length);


#endif /* BSP_USART_H_ */
