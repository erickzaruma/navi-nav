/*
 * bsp_encoder.c
 *
 *  Created on: Apr 29, 2025
 *      Author: Carlos
 */
#include "bsp_encoder.h"
#include "app_protocol.h"
#include "bsp_usart.h"
#include "bsp.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

int g_Encoder_M1_Now = 0;
int g_Encoder_M2_Now = 0;
int g_Encoder_M3_Now = 0;
int g_Encoder_M4_Now = 0;


int delta_Encoder_M1_Now = 0;
int delta_Encoder_M2_Now = 0;
int delta_Encoder_M3_Now = 0;
int delta_Encoder_M4_Now = 0;

#define LEN_DATA_BUFFER        21

static uint8_t data_buffer[LEN_DATA_BUFFER];  // persistent in memory
static uint8_t delta_data_buffer[LEN_DATA_BUFFER];  // persistent in memory


static int16_t Encoder_Read_CNT(uint8_t Motor_id)
{
	int16_t Encoder_TIM = 0;

	switch (Motor_id)
	{
	//case MOTOR_ID_M1: Encoder_TIM = (short)TIM2 -> CNT; TIM2 -> CNT = 0; break;
	//case MOTOR_ID_M2: Encoder_TIM = (short)TIM4 -> CNT; TIM4 -> CNT = 0; break;
	//case MOTOR_ID_M3: Encoder_TIM = (short)TIM5 -> CNT; TIM5 -> CNT = 0; break;
	//case MOTOR_ID_M4: Encoder_TIM = (short)TIM3 -> CNT; TIM3 -> CNT = 0; break;

	case MOTOR_ID_M1:  Encoder_TIM = 0x7fff - (short)TIM2 -> CNT; TIM2 -> CNT = 0x7fff; break;
	case MOTOR_ID_M2:  Encoder_TIM = 0x7fff - (short)TIM4 -> CNT; TIM4 -> CNT = 0x7fff; break;
	case MOTOR_ID_M3:  Encoder_TIM = 0x7fff - (short)TIM5 -> CNT; TIM5 -> CNT = 0x7fff; break;
	case MOTOR_ID_M4:  Encoder_TIM = 0x7fff - (short)TIM3 -> CNT; TIM3 -> CNT = 0x7fff; break;
	default: break;

	}
	return Encoder_TIM;
}

void Encoder_Get_ALL (int* Encoder_all)
{
	Encoder_all[0] = g_Encoder_M1_Now;
	Encoder_all[1] = g_Encoder_M2_Now;
	Encoder_all[2] = g_Encoder_M3_Now;
	Encoder_all[3] = g_Encoder_M4_Now;
}


int Encoder_Get_Count_Now(uint8_t Motor_id)
{
	if (Motor_id == MOTOR_ID_M1) return g_Encoder_M1_Now;
	if (Motor_id == MOTOR_ID_M2) return g_Encoder_M2_Now;
	if (Motor_id == MOTOR_ID_M3) return g_Encoder_M3_Now;
	if (Motor_id == MOTOR_ID_M4) return g_Encoder_M4_Now;
	return 0;
}


// Update the count value of the encoder
void Encoder_Update_Count(void)
{
	g_Encoder_M1_Now += Encoder_Read_CNT(MOTOR_ID_M1);
//	g_Encoder_M1_Now -= Encoder_Read_CNT(MOTOR_ID_M1);

//	g_Encoder_M2_Now += Encoder_Read_CNT(MOTOR_ID_M2);
	g_Encoder_M2_Now -= Encoder_Read_CNT(MOTOR_ID_M2);

//	g_Encoder_M3_Now += Encoder_Read_CNT(MOTOR_ID_M3);
	g_Encoder_M3_Now -= Encoder_Read_CNT(MOTOR_ID_M3);

	g_Encoder_M4_Now += Encoder_Read_CNT(MOTOR_ID_M4);
//	g_Encoder_M4_Now -= Encoder_Read_CNT(MOTOR_ID_M4);

}

// Initializing timer
void Encoder_Init(void)
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM2 -> CNT = 0x7fff;
	TIM4 -> CNT = 0x7fff;
	TIM5 -> CNT = 0x7fff;
	TIM3 -> CNT = 0x7fff;

}



void Encoder_Send_Count_Now(void)
{

	//uint8_t data_buffer[LEN] = {0};
	uint8_t i, checknum = 0;
	data_buffer[0]  = PTO_HEAD;
	data_buffer[1]  = PTO_DEVICE_ID-1;
	data_buffer[2]  = LEN_DATA_BUFFER-2;
	data_buffer[3]  = FUNC_REPORT_ENCODER;
	data_buffer[4]  = g_Encoder_M1_Now & 0xff;
	data_buffer[5]  = (g_Encoder_M1_Now >> 8) & 0xff;
	data_buffer[6]  = (g_Encoder_M1_Now >> 16) & 0xff;
	data_buffer[7]  = (g_Encoder_M1_Now >> 24) & 0xff;
	data_buffer[8]  = g_Encoder_M2_Now & 0xff;
	data_buffer[9]  = (g_Encoder_M2_Now >> 8) & 0xff;
	data_buffer[10] = (g_Encoder_M2_Now >> 16) & 0xff;
	data_buffer[11] = (g_Encoder_M2_Now >> 24) & 0xff;
	data_buffer[12] = g_Encoder_M3_Now & 0xff;
	data_buffer[13] = (g_Encoder_M3_Now >> 8) & 0xff;
	data_buffer[14] = (g_Encoder_M3_Now >> 16) & 0xff;
	data_buffer[15] = (g_Encoder_M3_Now >> 24) & 0xff;
	data_buffer[16] = g_Encoder_M4_Now & 0xff;
	data_buffer[17] = (g_Encoder_M4_Now >> 8) & 0xff;
	data_buffer[18] = (g_Encoder_M4_Now >> 16) & 0xff;
	data_buffer[19] = (g_Encoder_M4_Now >> 24) & 0xff;



	for (i = 2; i < LEN_DATA_BUFFER-1; i++)
	{
		checknum += data_buffer[i];
	}
	data_buffer[LEN_DATA_BUFFER -1] = checknum;


	USART1_Send_ArrayU8(data_buffer, sizeof(data_buffer));


}


void Encoder_Send_Velocity_Now(int encoder_current[4], int encoder_previous[4], uint32_t elapsed_time)
{
	int delta_encoder[4];

	for (int i = 0; i < 4; i++) {
	    delta_encoder[i] = encoder_current[i] - encoder_previous[i];
	}

	int delta_Encoder_M1_Now = delta_encoder[0];
	int delta_Encoder_M2_Now = delta_encoder[1];
	int delta_Encoder_M3_Now = delta_encoder[2];
	int delta_Encoder_M4_Now = delta_encoder[3];

		uint8_t i, checknum = 0;
		delta_data_buffer[0]  = PTO_HEAD;
		delta_data_buffer[1]  = PTO_DEVICE_ID-1;
		delta_data_buffer[2]  = LEN_DATA_BUFFER-2;
		delta_data_buffer[3]  = FUNC_REPORT_ENCODER;
		delta_data_buffer[4]  = delta_Encoder_M1_Now & 0xff;
		delta_data_buffer[5]  = (delta_Encoder_M1_Now >> 8) & 0xff;
		delta_data_buffer[6]  = (delta_Encoder_M1_Now >> 16) & 0xff;
		delta_data_buffer[7]  = (delta_Encoder_M1_Now >> 24) & 0xff;
		delta_data_buffer[8]  = delta_Encoder_M2_Now & 0xff;
		delta_data_buffer[9]  = (delta_Encoder_M2_Now >> 8) & 0xff;
		delta_data_buffer[10] = (delta_Encoder_M2_Now >> 16) & 0xff;
		delta_data_buffer[11] = (delta_Encoder_M2_Now >> 24) & 0xff;
		delta_data_buffer[12] = delta_Encoder_M3_Now & 0xff;
		delta_data_buffer[13] = (delta_Encoder_M3_Now >> 8) & 0xff;
		delta_data_buffer[14] = (delta_Encoder_M3_Now >> 16) & 0xff;
		delta_data_buffer[15] = (delta_Encoder_M3_Now >> 24) & 0xff;
		delta_data_buffer[16] = delta_Encoder_M4_Now & 0xff;
		delta_data_buffer[17] = (delta_Encoder_M4_Now >> 8) & 0xff;
		delta_data_buffer[18] = (delta_Encoder_M4_Now >> 16) & 0xff;
		delta_data_buffer[19] = (delta_Encoder_M4_Now >> 24) & 0xff;



		for (i = 2; i < LEN_DATA_BUFFER-1; i++)
		{
			checknum += delta_data_buffer[i];
		}
		delta_data_buffer[LEN_DATA_BUFFER -1] = checknum;


		USART1_Send_ArrayU8(delta_data_buffer, sizeof(delta_data_buffer));
}



