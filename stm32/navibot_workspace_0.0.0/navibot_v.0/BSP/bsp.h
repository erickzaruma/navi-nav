/*
 * bsp.h
 *
 *  Created on: Apr 29, 2025
 *      Author: Muyu Innovations
 */

#ifndef BSP_H_
#define BSP_H_

#include "main.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_usart.h"
#include "bsp_icm20948.h"

#include <stdio.h>

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

void Bsp_Init(void);

#endif /* BSP_H_ */
