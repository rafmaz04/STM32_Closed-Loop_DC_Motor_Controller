/*
 * car_control.h
 *
 *  Created on: Jun 30, 2026
 *      Author: Rafal
 */

#ifndef INC_CAR_CONTROL_H_
#define INC_CAR_CONTROL_H_

typedef enum {
    PARK    = 0,
    DRIVE   = 1,
    REVERSE = 2
} CarState_t;


void Car_ApplyMotorPower(float pi_output);

void DriveRoutine(void);

void ParkRoutine(void);

void ReverseRoutine(void);

#endif /* INC_CAR_CONTROL_H_ */
