/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void CalcullateTilts(void);
extern void CANFilters(CAN_HandleTypeDef *hcan);
extern void TrmCAN(CAN_HandleTypeDef *hcan);
extern void ProcessCANReceivedPacket(CAN_HandleTypeDef *hcan);
extern void FlashReadAll(void);
extern void FlashWriteAll(void);
extern void Led(void);


extern float AccXfl, AccYfl, AccZfl,TiltX, TiltY;
extern float GyroXfl, GyroYfl, GyroZfl,KGyro,KAcc;
extern uint16_t ThisAddr;
extern int16_t AccXZero, AccYZero, AccZZero;
extern int16_t GyroXZero, GyroYZero, GyroZZero;
extern uint8_t Calibrate,LedStateMachine;


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GreenLed_Pin GPIO_PIN_15
#define GreenLed_GPIO_Port GPIOC
#define MPU6500CS_Pin GPIO_PIN_0
#define MPU6500CS_GPIO_Port GPIOB
#define CAB_STBY_Pin GPIO_PIN_10
#define CAB_STBY_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define MPU6500_CS GPIOB, GPIO_PIN_0


#define TICK_PERIOD 	0.004

//Flash addrs:

#define START_ADDR_USER_PAGE           ((uint32_t)0x08007800)


#define FLASH_ADDR_AccXZero		0
#define FLASH_ADDR_AccYZero		4
#define FLASH_ADDR_AccZZero		8
#define FLASH_ADDR_GyroXZero	12
#define FLASH_ADDR_GyroYZero	16
#define FLASH_ADDR_GyroZZero	20
#define FLASH_ADDR_ThisAddr		24
#define FLASH_ADDR_KGyro		28







#define SMPLRT_DIV		25
#define CONFIG			26
#define GYRO_CONFIG		27
#define ACCEL_CONFIG	28
#define ACCEL_CONFIG_2	29
#define LP_ACCEL_ODR	30
#define FIFO_EN			35

#define INT_PIN_CFG		55
#define INT_ENABLE		56
#define INT_STATUS		58

#define INT_STATUS		58
#define ACCEL_XOUT_H	59
#define ACCEL_XOUT_L	60
#define ACCEL_YOUT_H	61
#define ACCEL_YOUT_L	62
#define ACCEL_ZOUT_H	63
#define ACCEL_ZOUT_L	64
#define TEMP_OUT_H		65
#define TEMP_OUT_L		66
#define GYRO_XOUT_H		67
#define GYRO_XOUT_L		68
#define GYRO_YOUT_H		69
#define GYRO_YOUT_L		70
#define GYRO_ZOUT_H		71
#define GYRO_ZOUT_L		72

#define SIGNAL_PATH_RESET 104
#define USER_CTRL		106
#define PWR_MGMT_1		107
#define PWR_MGMT_2		108


#define SIZE_OF_REC_ARRAY	100


#define SET_ADDR_CAN	0xfe


#define CALIBRATE		1


#define GreenLed GreenLed_GPIO_Port, GreenLed_Pin
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
