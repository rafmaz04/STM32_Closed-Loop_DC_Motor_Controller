/*
 * filter.c
 *
 *  Created on: Jul 1, 2026
 *      Author: Rafal
 */


#include "filter.h"


void  EMA_Init(EMA_Filter_t *f, float alpha, float initial_value)
{
	f->alpha = alpha;
	f->value = initial_value;
}
float EMA_Update(EMA_Filter_t *f, float input)
{
	f->value = (f->alpha * input) + ((1.0 - f->alpha) * f->value);
	return f->value;
}

void EMA_Reset(EMA_Filter_t *f)
{
	f->value = 0.0f;
}
