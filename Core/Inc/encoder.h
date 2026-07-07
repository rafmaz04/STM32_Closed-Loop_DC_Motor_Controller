/*
 * speed_sensor.h
 *
 *  Created on: Jul 1, 2026
 *      Author: Rafal
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"

typedef struct {
	TIM_HandleTypeDef 	*htim;
    uint16_t 			current_count;
    uint16_t 			previous_count;
    int16_t  			delta_count;
    float   			ticks_to_rpm;
} Encoder_t;

void Encoder_Init(Encoder_t *enc, TIM_HandleTypeDef *htim, float ticks_to_rpm);

float Encoder_Update(Encoder_t *enc);

#endif /* INC_ENCODER_H_ */
