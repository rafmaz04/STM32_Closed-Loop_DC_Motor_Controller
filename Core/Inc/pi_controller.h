/*
 * pi_controller.h
 *
 *  Created on: Jun 30, 2026
 *      Author: Rafal
 */

#ifndef INC_PI_CONTROLLER_H_
#define INC_PI_CONTROLLER_H_

typedef struct {
    float Kp;
    float Ki;
    float Kb;
    float max_output;

    float yi;
    float yp;
    float output_BS;
    float output_AS;
    float error;

} PI_Controller_t;

void PI_Init(PI_Controller_t *pi, float Kp, float Ki, float Kb, float max_output);
float PI_Update(PI_Controller_t *pi, float reference, float measured);
void PI_Reset(PI_Controller_t *pi);

#endif /* INC_PI_CONTROLLER_H_ */
