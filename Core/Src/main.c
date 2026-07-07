/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pi_controller.h"
#include "car_control.h"
#include "filter.h"
#include "encoder.h"
#include "ramp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_PI_OUTPUT			800
#define MAX_MOTOR_SPEED			170.0f
#define RPM_PER_STEP			0.5f
#define ALPHA					0.2f
#define RPM_CONVERSION_FACTOR	1.5625f

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile CarState_t CAR_STATE = PARK;

PI_Controller_t motor_pi;
Encoder_t		motor_encoder;
EMA_Filter_t	rpm_filter;
Ramp_t 			speed_ramp;

volatile uint32_t AdcValue;

volatile float motor_rpm      = 0.0f;
volatile float reference_speed = 0.0f;

float Kp = 16.2f;
float Ki = 0.07f;
float Kb = 2.0f;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);

  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

  HAL_ADCEx_InjectedStart_IT(&hadc2);

	// 10ms - frequency of interrupt
	// 16 ticks per revolution
	// 120:1 gearbox
	// 60* (10ms * 100) / (16 * 120) = 3.125
	//Arduino 2X Mode = 16 ticks per motor revolution.
	//STM32 4X Mode (TI12) = 32 ticks per motor revolution!
	// 60* (10ms * 100) / (32 * 120) = 1.5625
  Encoder_Init(&motor_encoder, &htim2, RPM_CONVERSION_FACTOR);
  EMA_Init(&rpm_filter, ALPHA, 0.0f);
  PI_Init(&motor_pi, Kp, Ki, Kb, MAX_PI_OUTPUT);
  Ramp_Init(&speed_ramp, RPM_PER_STEP);

  HAL_GPIO_WritePin(PARK_LED_GPIO_Port, PARK_LED_Pin, GPIO_PIN_SET);

  HAL_TIM_Base_Start(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* ADC1_2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t last_press = 0;

    if (HAL_GetTick() - last_press < 100)
    {
    	return;
    }

    last_press = HAL_GetTick();

    if (GPIO_Pin == BTN_UP_Pin)
    {
        if (CAR_STATE == REVERSE)
        {
            CAR_STATE = PARK;
            ParkRoutine();

        }
        else if (CAR_STATE == PARK)
        {
            CAR_STATE = DRIVE;
            DriveRoutine();
        }
    }
    else if (GPIO_Pin == BTN_DOWN_Pin)
    {
        if (CAR_STATE == DRIVE)
        {
            CAR_STATE = PARK;
            ParkRoutine();
        }
        else if (CAR_STATE == PARK)
        {
            CAR_STATE = REVERSE;
            ReverseRoutine();
        }
    }
}


void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if( hadc->Instance == ADC2 )
	{
		AdcValue = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);

        if (CAR_STATE == PARK || AdcValue < 60)
        {
            PI_Reset(&motor_pi);
            Ramp_Reset(&speed_ramp);
            EMA_Reset(&rpm_filter);
            motor_rpm      = 0.0f;
            reference_speed = 0.0f;
            Car_ApplyMotorPower(0.0f);


        }
        else
        {
        	float raw_rpm = Encoder_Update(&motor_encoder);
        	motor_rpm = EMA_Update(&rpm_filter, raw_rpm);

            float target = ((float)AdcValue / 4095.0f) * MAX_MOTOR_SPEED;


            if (CAR_STATE == REVERSE)
            {
            	target = -target;
            }


            reference_speed = Ramp_Update(&speed_ramp, target);

            float final_pi_output = PI_Update(&motor_pi, reference_speed, motor_rpm);
            Car_ApplyMotorPower(final_pi_output);

        }

	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
