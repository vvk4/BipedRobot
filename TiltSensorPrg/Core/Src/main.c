/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "eeprom.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
uint8_t SPI1ReceiveArray[100], MPU6500Initialized;
uint8_t UART2TransmittArray[200];
uint8_t CalibratingStateMachine, Calibrate;
int16_t AccX, AccY, AccZ;
int16_t GyroX, GyroY, GyroZ;
int16_t AccXZero, AccYZero, AccZZero;
int16_t GyroXZero, GyroYZero, GyroZZero;
int32_t SummAccX, SummAccY, SummAccZ, SummGyroX, SummGyroY, SummGyroZ;
int16_t CalibrateCnt;
float AccXfl, AccYfl, AccZfl;
float GyroXfl, GyroYfl, GyroZfl;
int16_t Temperature, CntSPI;
volatile uint32_t TimestampCounter, Timestamp;
uint8_t PacketRec[SIZE_OF_REC_ARRAY], MPU9250PacketReady;
uint16_t ThisAddr;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM6_Init(void);
static void MX_CAN_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void InitMPU6500(void);
uint32_t read_byte(uint32_t *addr);
uint8_t ReadRegisterMPU6500(uint8_t Addr);
void WriteRegisterMPU6500(uint8_t Addr, uint8_t Data);
void ProcessDataMPU6500(void);
void Calibrating(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	FlashReadAll();
	LedStateMachine = 7;

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM6_Init();
	MX_CAN_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */

	InitMPU6500();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	while (1) {

		if (MPU9250PacketReady) {
			MPU9250PacketReady = 0;
			ProcessDataMPU6500();
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN_Init(void) {

	/* USER CODE BEGIN CAN_Init 0 */

	/* USER CODE END CAN_Init 0 */

	/* USER CODE BEGIN CAN_Init 1 */
	/* USER CODE END CAN_Init 1 */
	hcan.Instance = CAN;
	hcan.Init.Prescaler = 5;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_4TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = ENABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN CAN_Init 2 */

	CANFilters(&hcan);

	/* USER CODE END CAN_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void) {

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 9;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 7200;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */
	htim6.Instance->DIER |= TIM_DIER_UIE;
	htim6.Instance->CR1 |= TIM_CR1_CEN;

	/* USER CODE END TIM6_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	/* DMA1_Channel3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(MPU6500CS_GPIO_Port, MPU6500CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(CAB_STBY_GPIO_Port, CAB_STBY_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : GreenLed_Pin */
	GPIO_InitStruct.Pin = GreenLed_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GreenLed_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : MPU6500CS_Pin */
	GPIO_InitStruct.Pin = MPU6500CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MPU6500CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CAB_STBY_Pin */
	GPIO_InitStruct.Pin = CAB_STBY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CAB_STBY_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void InitMPU6500(void) {
	uint8_t Ch;

	//WriteRegisterMPU6500(CTRL_REG3,0x1);//soft reset

	Ch = ReadRegisterMPU6500(117);
	asm("Nop");

	WriteRegisterMPU6500(PWR_MGMT_1, 0x80);
	HAL_Delay(110);
	WriteRegisterMPU6500(SIGNAL_PATH_RESET, 0x07);
	HAL_Delay(110);
	WriteRegisterMPU6500(USER_CTRL, 0x10);
	HAL_Delay(110);

	WriteRegisterMPU6500(ACCEL_CONFIG, 0x0);
	WriteRegisterMPU6500(ACCEL_CONFIG_2, 0x0);
	WriteRegisterMPU6500(GYRO_CONFIG, 0x0);

	WriteRegisterMPU6500(PWR_MGMT_1, 0x0);
	WriteRegisterMPU6500(PWR_MGMT_2, 0x0);
	//WriteRegisterMPU6500(LP_ACCEL_ODR, 10);

	WriteRegisterMPU6500(FIFO_EN, 0x0);
	WriteRegisterMPU6500(CONFIG, 0x0);

	WriteRegisterMPU6500(INT_PIN_CFG, 0x30);
	WriteRegisterMPU6500(INT_ENABLE, 0x1);

	WriteRegisterMPU6500(SMPLRT_DIV, 0x0);

	MPU6500Initialized = 1;
}

void WriteRegisterMPU6500(uint8_t Addr, uint8_t Data) {
	uint8_t TrmBytes[2];

	HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_RESET);
	TrmBytes[0] = Addr;
	TrmBytes[1] = Data;
	HAL_SPI_Transmit(&hspi1, TrmBytes, 2, 1000);
	HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_SET);
}

uint8_t ReadRegisterMPU6500(uint8_t Addr) {
	uint8_t TrmByte, RecByte;

	HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_RESET);
	TrmByte = Addr;
	TrmByte |= 0x80;
	HAL_SPI_Transmit(&hspi1, &TrmByte, 1, 1000);
	HAL_SPI_Receive(&hspi1, &RecByte, 1, 1000);

	HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_SET);

	return RecByte;
}

void ProcessDataMPU6500(void) {

	AccX = (((int16_t) SPI1ReceiveArray[1]) << 8)
			+ (int16_t) SPI1ReceiveArray[2];
	AccY = (((int16_t) SPI1ReceiveArray[3]) << 8)
			+ (int16_t) SPI1ReceiveArray[4];
	AccZ = (((int16_t) SPI1ReceiveArray[5]) << 8)
			+ (int16_t) SPI1ReceiveArray[6];

	Temperature = (((int16_t) SPI1ReceiveArray[7]) << 8)
			+ (int16_t) SPI1ReceiveArray[8];

	GyroX = (((int16_t) SPI1ReceiveArray[9]) << 8)
			+ (int16_t) SPI1ReceiveArray[10];
	GyroY = (((int16_t) SPI1ReceiveArray[11]) << 8)
			+ (int16_t) SPI1ReceiveArray[12];
	GyroZ = (((int16_t) SPI1ReceiveArray[13]) << 8)
			+ (int16_t) SPI1ReceiveArray[14];

	Calibrating();

	AccXfl = AccXZero - AccX; //меняем знак оси, чтобы он соответствовал матрице поворота
	AccYfl = AccYZero - AccY; //меняем знак оси, чтобы он соответствовал матрице поворота
	AccZfl = AccZ - AccZZero;
	GyroXfl = GyroX - GyroXZero;
	GyroYfl = GyroY - GyroYZero;
	GyroZfl = GyroZ - GyroZZero;

	CalcullateTilts();

	TrmCAN(&hcan);

}

void Calibrating(void) {
	if (!Calibrate)
		return;

	switch (CalibratingStateMachine) {
	case 0:
		CalibrateCnt = 250;
		SummAccX = 0;
		SummAccY = 0;
		SummAccZ = 0;
		SummGyroX = 0;
		SummGyroY = 0;
		SummGyroZ = 0;
		CalibratingStateMachine = 1;
		break;
	case 1:
		CalibrateCnt--;
		if (!CalibrateCnt)
			CalibratingStateMachine = 2;
		break;
	case 2:
		SummAccX = SummAccX + AccX;
		SummAccY = SummAccY + AccY;
		SummAccZ = SummAccZ + AccZ;
		SummGyroX = SummGyroX + GyroX;
		SummGyroY = SummGyroY + GyroY;
		SummGyroZ = SummGyroZ + GyroZ;

		CalibrateCnt++;
		if (CalibrateCnt >= 64) {
			AccXZero = (int16_t) (SummAccX >> 6);
			AccYZero = (int16_t) (SummAccY >> 6);
			AccZZero = -16384 + (int16_t) (SummAccZ >> 6);
			GyroXZero = (int16_t) (SummGyroX >> 6);
			GyroYZero = (int16_t) (SummGyroY >> 6);
			GyroZZero = (int16_t) (SummGyroZ >> 6);
			CalibratingStateMachine = 0;
			Calibrate = 0;
			FlashWriteAll();
		}
		break;

	default:
		CalibratingStateMachine = 0;
		break;
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	ProcessCANReceivedPacket(hcan);

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (hspi == &hspi1) {
		HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_SET);
		MPU9250PacketReady = 1;
	}
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM7 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */
	uint8_t TrmByte;

	if (htim->Instance == TIM6) {
//	    HAL_GPIO_TogglePin(GreenLed);

		Led();

		if (CntSPI > 3) {
			CntSPI = 0;
			if (MPU6500Initialized) {
				TrmByte = ACCEL_XOUT_H;	//Start MPU6500 data transfer   250Hz
				TrmByte |= 0x80;
//				HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_SET);
				//			asm("Nop");
				HAL_GPIO_WritePin(MPU6500_CS, GPIO_PIN_RESET);

				HAL_SPI_TransmitReceive_DMA(&hspi1, &TrmByte, SPI1ReceiveArray,
						16);
				//			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); //светодиод
			}
		}

		CntSPI++;

	}

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM7) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
