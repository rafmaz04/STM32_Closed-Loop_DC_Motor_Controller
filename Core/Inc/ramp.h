/*
 * ramp.h
 *
 *  Created on: Jul 1, 2026
 *      Author: Rafal
 */

#ifndef INC_RAMP_H_
#define INC_RAMP_H_

typedef struct {
    float value;   // current ramped output
    float rate;    // max change per update step (RPM per 10ms)
} Ramp_t;

void  Ramp_Init(Ramp_t *r, float rate);
float Ramp_Update(Ramp_t *r, float target);
void  Ramp_Reset(Ramp_t *r);

#endif /* INC_RAMP_H_ */
