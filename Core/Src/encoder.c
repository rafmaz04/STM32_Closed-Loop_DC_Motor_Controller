/*
 * speed_sensor.c
 *
 *  Created on: Jul 1, 2026
 *      Author: Rafal
 */

#include "encoder.h"



void Encoder_Init(Encoder_t *enc, TIM_HandleTypeDef *htim, float ticks_to_rpm)
{
	enc->htim			= htim;
	enc->current_count  = 0;
	enc->previous_count = 0;
	enc->delta_count	= 0;
	enc->ticks_to_rpm	= ticks_to_rpm;
}

float Encoder_Update(Encoder_t *enc)
{
	enc->current_count  = __HAL_TIM_GET_COUNTER(enc->htim);
	enc->delta_count    = (int16_t)(enc->current_count - enc->previous_count);
	enc->previous_count = enc->current_count;

	return enc->delta_count * enc->ticks_to_rpm;
}
