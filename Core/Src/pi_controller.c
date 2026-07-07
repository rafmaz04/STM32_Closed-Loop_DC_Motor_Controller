/*
 * pi_controller.c
 *
 *  Created on: Jun 30, 2026
 *      Author: Rafal
 */

#include "pi_controller.h"


void PI_Init(PI_Controller_t *pi, float Kp, float Ki, float Kb, float max_output)
{
    pi->Kp         = Kp;
    pi->Ki         = Ki;
    pi->Kb         = Kb;
    pi->max_output = max_output;
    pi->yi         = 0.0;
    pi->yp         = 0.0;
    pi->output_BS  = 0.0;
    pi->output_AS  = 0.0;
    pi->error      = 0.0;
}

float PI_Update(PI_Controller_t *pi, float reference, float measured)
{
    // 1. Current Error & Proportional
    pi->error = reference - measured;
    pi->yp    = pi->error * pi->Kp;

    // 2. Tentative Current Integral (Add the new error FIRST)
    float yi_temp = pi->yi + (pi->error * pi->Ki);

    // 3. Calculate Output (Now using CURRENT proportional + CURRENT integral)
    pi->output_BS = yi_temp + pi->yp;

    // 4. Clamping (Saturation bounds)
    if (pi->output_BS >= pi->max_output)
    {
    	pi->output_AS = pi->max_output;
    }
    else if (pi->output_BS <= -pi->max_output)
    {
    	pi->output_AS = -pi->max_output;
    }
    else
    {
    	pi->output_AS = pi->output_BS;
    }

    // 5. Finalize Integral (Apply Anti-Windup using the clamped values)
    pi->yi = yi_temp - (pi->Kb * (pi->output_BS - pi->output_AS));

    return pi->output_AS;
}

void PI_Reset(PI_Controller_t *pi)
{
    pi->yi        = 0.0;
    pi->yp		  = 0.0;
    pi->output_BS = 0.0;
    pi->output_AS = 0.0;
    pi->error     = 0.0;
}
