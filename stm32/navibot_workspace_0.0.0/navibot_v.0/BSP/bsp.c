/*
 * bsp.c
 *
 *  Created on: Apr 29, 2025
 *      Author: Muyu Innovations
 */

#include "bsp.h"


void Bsp_Init(void)
{
	Motor_PWM_Init();
	Encoder_Init();
	USART1_Init();
	ICM20948_init();
    AK09916_init();
}


