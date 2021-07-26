#include "main.h"

void CAN_Config(CAN_HandleTypeDef *phcan, uint8_t FIFO_Num) {
	/*Configure the filter to receive ID frames in the specified range*/
	CAN_FilterTypeDef CAN_FilterType;
	CAN_FilterType.FilterBank = 0;                        //Filter group [0,13]
	CAN_FilterType.SlaveStartFilterBank = 0; //14;             //Start from the filter group [0,27]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterIdLow = 0x0000; //ID to be filtered low [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	CAN_FilterType.FilterMaskIdLow = 0x0000; //The lower 16 bits of the filter must not match
	CAN_FilterType.FilterFIFOAssignment = FIFO_Num; //The filter is associated to (0=RX_FIFO0/1=RX_FIFO1)
	CAN_FilterType.FilterMode = CAN_FILTERMODE_IDLIST; //Work in identifier mask bit mode
	CAN_FilterType.FilterScale = CAN_FILTERSCALE_32BIT; //The filter bit width is a single 32 bits
	CAN_FilterType.FilterActivation = ENABLE;             //Enable filter
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterType.FilterBank = 1;                        //Filter group [0,13]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_HIP_L_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_HIP_L_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterType.FilterBank = 2;                        //Filter group [0,13]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_LOWERLEG_L_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_LOWERLEG_L_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterType.FilterBank = 3;                        //Filter group [0,13]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_FOOT_L_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_FOOT_L_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterType.FilterBank = 4;                        //Filter group [0,13]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_HIP_R_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_HIP_R_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterType.FilterBank = 5;                        //Filter group [0,13]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_LOWERLEG_R_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_LOWERLEG_R_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterType.FilterBank = 6;                        //Filter group [0,13]
	CAN_FilterType.FilterIdHigh = (TILT_SENSOR_FOOT_R_ADDR) << 5; //ID high bit to be filtered [0x0000,0xFFFF]
	CAN_FilterType.FilterMaskIdHigh = (TILT_SENSOR_FOOT_R_ADDR + 1) << 5; //0xffe0;             //The high 16 bits of the filter must not match
	if (HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK) {
		Error_Handler();
	}

	/*  CAN_FilterType.FilterBank = 11;                        //Filter group [0,13]
	 CAN_FilterType.FilterIdHigh = 0x2<<5;                 //ID high bit to be filtered [0x0000,0xFFFF]

	 if(HAL_CAN_ConfigFilter(phcan, &CAN_FilterType) != HAL_OK)
	 {
	 Error_Handler();
	 }

	 */

	/*Open the interrupt service corresponding to the CAN channel*/
	if (FIFO_Num == 0) {
		if (HAL_CAN_ActivateNotification(phcan, CAN_IT_RX_FIFO0_MSG_PENDING)
				!= HAL_OK) {
			Error_Handler();
		}
	} else {
		if (HAL_CAN_ActivateNotification(phcan, CAN_IT_RX_FIFO1_MSG_PENDING)
				!= HAL_OK) {
			Error_Handler();
		}
	}

	/*Start CAN communication*/
	if (HAL_CAN_Start(phcan) != HAL_OK) {
		Error_Handler();
	}
}

uint8_t Can_TxMessage(CAN_HandleTypeDef *phcan, uint8_t ide, uint32_t id,
		uint8_t len, uint8_t *pdata) {
	uint32_t TxMailbox;           //Get the email number that was sent
	CAN_TxHeaderTypeDef TxHeader; //Send-header protocol information structure, used to fill parameters
	HAL_StatusTypeDef HAL_RetVal; //CAN return value
	uint16_t i = 0;
	/*Fill and send header protocol*/
	if (ide == 0) {
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.StdId = id;
	} else {
		TxHeader.IDE = CAN_ID_EXT;
		TxHeader.ExtId = id;
	}

	TxHeader.RTR = CAN_RTR_DATA,      //The frame type of the message data frame
	TxHeader.DLC = len,                   //The length of the frame 8
	TxHeader.TransmitGlobalTime = DISABLE; //Do not capture time

	/*Ask if CAN has a free mailbox*/
	while (HAL_CAN_GetTxMailboxesFreeLevel(phcan) == 0) {
		i++;
		if (i > 0xfffe) //Timeout, sending failed
			return 1;
	}

	/*Send frame*/
	HAL_RetVal = HAL_CAN_AddTxMessage(phcan, &TxHeader, pdata, &TxMailbox); //Send a frame of data
	//printf("TxMailbox %d\r\n",TxMailbox);
	if (HAL_RetVal != HAL_OK)
		return 1;
	return 0;
}

void processCAN2Packet(CAN_HandleTypeDef *hcan) {
	uint8_t Rxbuff[8];
	CAN_RxHeaderTypeDef RxHeader;
	HAL_StatusTypeDef HAL_RetVal;

	HAL_RetVal = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Rxbuff); //Read a frame from channel 0 buffer
	if (HAL_RetVal == HAL_OK) {
		switch (RxHeader.StdId) {
		case TILT_SENSOR_ADDR:
			Tilt1X = *(float*) Rxbuff;
			Tilt1Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_ADDR + 1):
			Gyro1 = *(float*) Rxbuff;
			Gyro1Y = *(float*) &Rxbuff[4];
			//KGyro1=*(uint16_t*)&Rxbuff[4];
			break;
		case TILT_SENSOR_HIP_L_ADDR:
			Tilt2X = *(float*) Rxbuff;
			Tilt2Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_HIP_L_ADDR + 1):
			Gyro2 = *(float*) Rxbuff;
			Gyro2Y = *(float*) &Rxbuff[4];
			break;
		case TILT_SENSOR_LOWERLEG_L_ADDR:
			Tilt3X = *(float*) Rxbuff;
			Tilt3Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_LOWERLEG_L_ADDR + 1):
			Gyro3 = *(float*) Rxbuff;
			break;
		case TILT_SENSOR_FOOT_L_ADDR:
			Tilt4X = *(float*) Rxbuff;
			Tilt4Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_FOOT_L_ADDR + 1):
			Gyro4 = *(float*) Rxbuff;
			break;
		case TILT_SENSOR_HIP_R_ADDR:
			Tilt5X = *(float*) Rxbuff;
			Tilt5Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_HIP_R_ADDR + 1):
			Gyro5 = *(float*) Rxbuff;
			Gyro5Y = *(float*) &Rxbuff[4];
			break;
		case TILT_SENSOR_LOWERLEG_R_ADDR:
			Tilt6X = *(float*) Rxbuff;
			Tilt6Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_LOWERLEG_R_ADDR + 1):
			Gyro6 = *(float*) Rxbuff;
			break;
		case TILT_SENSOR_FOOT_R_ADDR:
			Tilt7X = *(float*) Rxbuff;
			Tilt7Y = *(float*) &Rxbuff[4];
			break;
		case (TILT_SENSOR_FOOT_R_ADDR + 1):
			Gyro7 = *(float*) Rxbuff;
			KGyro1 = *(uint16_t*) &Rxbuff[4];
			break;
		}
	}
}

