/*
 * ramp.c
 *
 *  Created on: Jul 1, 2026
 *      Author: Rafal
 */


#include "ramp.h"

void Ramp_Init(Ramp_t *r, float rate)
{
    r->rate  = rate;
    r->value = 0.0;
}

float Ramp_Update(Ramp_t *r, float target)
{
    float diff = target - r->value;

    if (diff > r->rate)
    {
    	r->value += r->rate;
    }
    else if (diff < -r->rate)
    {
    	r->value -= r->rate;
    }
    else
    {
    	r->value = target;
    }

    return r->value;
}

void Ramp_Reset(Ramp_t *r)
{
    r->value = 0.0;
}
