/*
 * bsp_motor.h
 *
 *  Created on: Apr 29, 2025
 *      Author: Carlos
 */

#include "main.h"

#ifndef BSP_MOTOR_H_
#define BSP_MOTOR_H_

#define PWM_M1_A TIM8->CCR1
#define PWM_M1_B TIM8->CCR2

#define PWM_M2_A TIM8->CCR3
#define PWM_M2_B TIM8->CCR4

#define PWM_M3_A TIM1->CCR4
#define PWM_M3_B TIM1->CCR1

#define PWM_M4_A TIM1->CCR2
#define PWM_M4_B TIM1->CCR3

#define MOTOR_ENABLE_A      (0x01)
#define MOTOR_ENABLE_B      (0x02)
#define MOTOR_ENABLE_C      (0x04)
#define MOTOR_ENABLE_D      (0x08)

#define MOTOR_SUNRISE_IGNORE_PULSE  (2000)
#define MOTOR_IGNORE_PULSE          (1600)
#define MOTOR_MAX_PULSE             (3600)
#define MOTOR_FREQ_DIVIDE           (0)


// MOTOR: M1 M2 M3 M4
//        |  |  |  |
// MOTOR: L1 L2 R1 R2

typedef enum {
    MOTOR_ID_M1 = 0,
    MOTOR_ID_M2,
    MOTOR_ID_M3,
    MOTOR_ID_M4,
    MAX_MOTOR
} Motor_ID;


void Motor_PWM_Init(void);

void Motor_Set_Pwm(uint8_t id, int16_t speed); //
void Motor_Stop(uint8_t brake);  //
void Motor_GPIO_Init(void);

void Motion_Set_Pwm(int16_t Motor_1, int16_t Motor_2, int16_t Motor_3, int16_t Motor_4);

uint8_t Motor_Get_Enable_State(uint8_t id);

void Motor_Check_Start(void);
int Motor_Check_Result(uint8_t Encoder_id);

void Motor_Close_Brake(void);

#endif /* BSP_MOTOR_H_ */
