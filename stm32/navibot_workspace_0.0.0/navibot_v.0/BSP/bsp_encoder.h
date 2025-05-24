/*
 * bsp_encoder.h
 *
 *  Created on: Apr 29, 2025
 *      Author: Carlos
 */

#ifndef BSP_ENCODER_H_
#define BSP_ENCODER_H_

#include "main.h"
#include "stdint.h"


// One full turn of wheel, the number of pulses picked up by the coder 30*11*2*2 (1320)
#define ENCODER_CIRCLE           1320.0f

#define ENCODER_TIM_PERIOD       (uint16_t)(65535)

void Encoder_Init(void);
void Encoder_Update_Count(void);
int Encoder_Get_Count_Now(uint8_t Motor_id);
void Encoder_Get_ALL(int* Encoder_all);

void Encoder_Send_Count_Now(void);

void Encoder_Send_Velocity_Now(int encoder_current[4], int encoder_previous[4], uint32_t elapsed_time);

#endif /* BSP_ENCODER_H_ */
