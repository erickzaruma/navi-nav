/*
 * app_protocol.c
 *
 *  Created on: May 1, 2025
 *      Author: Muyu Innovations
 */

#define ENABLE_CHECKSUM 1

#include "app_protocol.h"
#include "bsp.h"
#include "bsp_motor.h"


uint8_t RxBuffer[PTO_MAX_BUF_LEN]; // Command receive buffer

uint8_t RxIndex = 0; // Receive data index
uint8_t RxFlag = 0;// Receive state machine
uint8_t New_CMD_flag;// New command received flag
uint8_t New_CMD_length;// Length of the new command data

/* Data request flag */
uint8_t g_Request_Flag = 0;
uint8_t g_Request_ARM_ID = 0;
uint8_t g_Request_Parm = 0;

void Upper_Data_Receive(uint8_t Rx_Temp)
{
	switch (RxFlag)
	{
	case 0:
		if (Rx_Temp == PTO_HEAD)
		{
			RxBuffer[0] = PTO_HEAD;
			RxFlag = 1;

			Encoder_Send_Count_Now();
		}
		break;

	case 1:
		if (Rx_Temp == PTO_DEVICE_ID)
		{
			RxBuffer[1] = PTO_DEVICE_ID;
			RxFlag = 2;
			RxIndex = 2;

		}
		else
		{
			RxFlag = 0;
			RxBuffer[0] = 0x0;
		}
		break;

	case 2:
		New_CMD_length = Rx_Temp + 2;
		if (New_CMD_length >= PTO_MAX_BUF_LEN)
		{
			RxIndex = 0;
			RxFlag = 0;
			RxBuffer[0] = 0;
			RxBuffer[1] = 0;
			New_CMD_length = 0;
			break;
		}
		RxBuffer[RxIndex] = Rx_Temp;
		RxIndex++;
		RxFlag = 3;
		break;

	case 3:
		RxBuffer[RxIndex] = Rx_Temp;
		RxIndex++;
		if (RxIndex >= New_CMD_length)
		{
			New_CMD_flag = 1;
			RxIndex = 0;
			RxFlag = 0;
		}
		break;

	default:
		break;
	}
}

void Upper_Data_Parse(uint8_t *data_buf, uint8_t num)
{
	#if ENABLE_CHECKSUM

	int sum = 0;
	for (uint8_t i = 2; i < (num - 1); i++)
		sum += *(data_buf + i);

	sum = sum & 0xFF;

	uint8_t recvSum = *(data_buf + num - 1);

	if (!(sum == recvSum))
	{
		return;
	}
	#endif

	uint8_t func_id = *(data_buf + 3);

	switch (func_id)
	{
	case FUNC_MOTOR:
		{
			int16_t speed[4] = {0};
			int8_t motor_1 = *(data_buf + 4);
			int8_t motor_2 = *(data_buf + 5);
			int8_t motor_3 = *(data_buf + 6);
			int8_t motor_4 = *(data_buf + 7);


			int16_t motor_pulse = MOTOR_MAX_PULSE - MOTOR_IGNORE_PULSE;

			speed[0] = (int16_t)motor_1 * (motor_pulse / 100.0);
			speed[1] = (int16_t)motor_2 * (motor_pulse / 100.0);
			speed[2] = (int16_t)motor_3 * (motor_pulse / 100.0);
			speed[3] = (int16_t)motor_4 * (motor_pulse / 100.0);

			Motion_Set_Pwm(speed[0], speed[1], speed[2], speed[3]);

			break;
		}
	default:
			break;
		}

	}





