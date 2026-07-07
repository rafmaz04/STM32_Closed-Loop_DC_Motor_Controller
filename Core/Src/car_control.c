/*
 * car_control.c
 *
 *  Created on: Jun 30, 2026
 *      Author: Rafal
 */
#include "car_control.h"
#include "main.h"

#define MIN_CCR				1000
#define MAX_CCR				1800

extern TIM_HandleTypeDef htim8;
volatile uint32_t CCR_Value;

void Car_ApplyMotorPower(float pi_output)
{
    if( pi_output > 20.0)
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
        CCR_Value = (uint32_t)(pi_output + MIN_CCR);
        if (CCR_Value > MAX_CCR)
        {
        	CCR_Value = MAX_CCR;
        }
    }
    else if(pi_output < -20.0)
    {
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_SET);
        CCR_Value = (uint32_t)(-pi_output + MIN_CCR);
        if (CCR_Value > MAX_CCR)
        {
        	CCR_Value = MAX_CCR;
        }
    }
    else
    {
        CCR_Value = 0;
    }
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, CCR_Value);
}

void DriveRoutine(void)
{
    HAL_GPIO_WritePin(REVERSE_LED_GPIO_Port, REVERSE_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DRIVE_LED_GPIO_Port, DRIVE_LED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PARK_LED_GPIO_Port, PARK_LED_Pin, GPIO_PIN_RESET);
}

void ParkRoutine(void)
{
    HAL_GPIO_WritePin(REVERSE_LED_GPIO_Port, REVERSE_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DRIVE_LED_GPIO_Port, DRIVE_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PARK_LED_GPIO_Port, PARK_LED_Pin, GPIO_PIN_SET);
}

void ReverseRoutine(void)
{
    HAL_GPIO_WritePin(REVERSE_LED_GPIO_Port, REVERSE_LED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DRIVE_LED_GPIO_Port, DRIVE_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PARK_LED_GPIO_Port, PARK_LED_Pin, GPIO_PIN_RESET);
}

