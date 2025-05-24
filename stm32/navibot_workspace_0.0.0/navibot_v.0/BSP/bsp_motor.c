/*
 * bsp_motor.c
 *
 *  Created on: Apr 29, 2025
 *      Author: Muyu Innovations
 */

#include "bsp_motor.h"

uint8_t motor_enable = 0;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;


void Motor_PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

void Motor_Stop(uint8_t brake)
{
	if (brake !=0) brake = 1;
	PWM_M1_A = brake * MOTOR_MAX_PULSE;
	PWM_M1_B = brake * MOTOR_MAX_PULSE;
	PWM_M2_A = brake * MOTOR_MAX_PULSE;
	PWM_M2_B = brake * MOTOR_MAX_PULSE;
	PWM_M3_A = brake * MOTOR_MAX_PULSE;
	PWM_M3_B = brake * MOTOR_MAX_PULSE;
	PWM_M4_A = brake * MOTOR_MAX_PULSE;
	PWM_M4_B = brake * MOTOR_MAX_PULSE;

}

void Motor_GPIO_Init(void)
{

	motor_enable = MOTOR_ENABLE_A | MOTOR_ENABLE_B | MOTOR_ENABLE_C | MOTOR_ENABLE_D;
}


static int16_t Motor_Ignore_Dead_Zone(int16_t pulse)
{
	if (pulse > 0) return pulse + MOTOR_IGNORE_PULSE;
	if (pulse < 0) return pulse - MOTOR_IGNORE_PULSE;
	return 0;

}

void Motor_Set_Pwm(uint8_t id, int16_t speed)
{
    int16_t pulse = Motor_Ignore_Dead_Zone(speed);

    //LImit input
    if (pulse >= MOTOR_MAX_PULSE)
        pulse = MOTOR_MAX_PULSE;
    if (pulse <= -MOTOR_MAX_PULSE)
        pulse = -MOTOR_MAX_PULSE;

    switch (id)
    {
    case MOTOR_ID_M1:
    {
        pulse = -pulse;
        if (pulse >= 0)
        {
            PWM_M1_A = pulse;
            PWM_M1_B = 0;
        }
        else
        {
            PWM_M1_A = 0;
            PWM_M1_B = -pulse;
        }
        break;
    }
    case MOTOR_ID_M2:
    {
        pulse = -pulse;
        if (pulse >= 0)
        {
            PWM_M2_A = pulse;
            PWM_M2_B = 0;
        }
        else
        {
            PWM_M2_A = 0;
            PWM_M2_B = -pulse;
        }
        break;
    }

    case MOTOR_ID_M3:
    {
        if (pulse >= 0)
        {
            PWM_M3_A = pulse;
            PWM_M3_B = 0;
        }
        else
        {
            PWM_M3_A = 0;
            PWM_M3_B = -pulse;
        }
        break;
    }
    case MOTOR_ID_M4:
    {
        if (pulse >= 0)
        {
            PWM_M4_A = pulse;
            PWM_M4_B = 0;
        }
        else
        {
            PWM_M4_A = 0;
            PWM_M4_B = -pulse;
        }
        break;
    }

    default:
        break;
    }
}

uint8_t Motor_Get_Enable_State(uint8_t enable_m)
{
    return motor_enable & enable_m;
}


void Motion_Set_Pwm(int16_t Motor_1, int16_t Motor_2, int16_t Motor_3, int16_t Motor_4)
{
    if (Motor_1 >= -MOTOR_MAX_PULSE && Motor_1 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M1, Motor_1);
    }
    if (Motor_2 >= -MOTOR_MAX_PULSE && Motor_2 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M2, Motor_2);
    }
    if (Motor_3 >= -MOTOR_MAX_PULSE && Motor_3 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M3, Motor_3);
    }
    if (Motor_4 >= -MOTOR_MAX_PULSE && Motor_4 <= MOTOR_MAX_PULSE)
    {
        Motor_Set_Pwm(MOTOR_ID_M4, Motor_4);
    }
}
