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
#include "stm32f4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void ReceivePacketUART(UART_HandleTypeDef *huart);
extern uint8_t CalcCheckSumm(uint16_t N, uint8_t *Array);
extern void CAN_Config(CAN_HandleTypeDef *phcan, uint8_t FIFO_Num);
extern uint8_t Can_TxMessage(CAN_HandleTypeDef *phcan, uint8_t ide, uint32_t id, uint8_t len, uint8_t *pdata);
extern void processCAN2Packet(CAN_HandleTypeDef *hcan);
extern uint16_t TiltSensor1DataReady,TiltSensor2DataReady,TiltSensor3DataReady,TiltSensor4DataReady,TiltSensor5DataReady,TiltSensor6DataReady,TiltSensor7DataReady;
extern void TrmDataPacketUART(UART_HandleTypeDef *huart);
extern void SetMotorPWM(int16_t PWM,uint16_t MotorNum);
extern TIM_HandleTypeDef htim1,htim2,htim3,htim4,htim8,htim12;
extern void PWM_Off(void);

enum CommandsToPC {TRM_DATA_PACKET=1};
enum MotorsNum {MOTOR_L1=1,MOTOR_L2,MOTOR_L3,MOTOR_L4,MOTOR_R1,MOTOR_R2,MOTOR_R3,MOTOR_R4};
#define SIZE_OF_RECARRAY 250
extern uint16_t CntRxTimeOut,KGyro1;
extern uint8_t GettingPacketRx_FL,RecArray[SIZE_OF_RECARRAY],PacketReceived_FL;
extern CAN_HandleTypeDef hcan2;
extern float Tilt1X,Tilt1Y,Gyro1;
extern float Tilt2X,Tilt2Y,Gyro2;
extern float Tilt3X,Tilt3Y,Gyro3;
extern float Tilt4X,Tilt4Y,Gyro4;
extern float Tilt5X,Tilt5Y,Gyro5;
extern float Tilt6X,Tilt6Y,Gyro6;
extern float Tilt7X,Tilt7Y,Gyro7;
extern uint8_t UART2TransmittArray[256];
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

#define MAX_PWM				3500



#define TILT_SENSOR_ADDR1	1
#define TILT_SENSOR_ADDR2	3
#define TILT_SENSOR_ADDR3	5
#define TILT_SENSOR_ADDR4	7
#define TILT_SENSOR_ADDR5	9
#define TILT_SENSOR_ADDR6	11
#define TILT_SENSOR_ADDR7	13





#define SET_ADDR  			1
#define CALIBR1 			2
#define CALIBR2 			3
#define CALIBR3 			4
#define CALIBR4 			5
#define CALIBR5 			6
#define CALIBR6 			7
#define CALIBR7 			8
#define SET_KGYRO  			9
#define SET_PWM_Motor_L1   10
#define SET_PWM_Motor_L2   11
#define SET_PWM_Motor_L3   12
#define SET_PWM_Motor_L4   13
#define SET_PWM_Motor_R1   14
#define SET_PWM_Motor_R2   15
#define SET_PWM_Motor_R3   16
#define SET_PWM_Motor_R4   17


#define SET_ADDR_CAN		0xfe


#define CALIBRATE		1


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
