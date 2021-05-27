#include "main.h"

void CANFilters(CAN_HandleTypeDef *hcan);
void TrmCAN(CAN_HandleTypeDef *hcan);
void ProcessCANReceivedPacket(CAN_HandleTypeDef *hcan);

uint8_t j;

void CANFilters(CAN_HandleTypeDef *hcan) {
	CAN_FilterTypeDef canFilterConfig;

	canFilterConfig.FilterBank = 0;
	canFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfig.FilterIdHigh = (ThisAddr + 0x100) << 5;
	canFilterConfig.FilterIdLow = 0x0000;
	canFilterConfig.FilterMaskIdHigh = 0x0000;
	canFilterConfig.FilterMaskIdLow = 0x0000;
	canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfig.FilterActivation = ENABLE;
	canFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(hcan, &canFilterConfig);

	canFilterConfig.FilterBank = 1;
	canFilterConfig.FilterIdHigh = (SET_ADDR_CAN) << 5;
	HAL_CAN_ConfigFilter(hcan, &canFilterConfig);

	HAL_CAN_Start(hcan);
	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

}

void TrmCAN(CAN_HandleTypeDef *hcan) {
	uint32_t N;
	CAN_TxHeaderTypeDef msgHeader;
	uint8_t msgData[8];

	N = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	if (N > 1) {
		msgHeader.StdId = ThisAddr;
		msgHeader.DLC = 8;
		msgHeader.TransmitGlobalTime = DISABLE;
		msgHeader.RTR = CAN_RTR_DATA;
		msgHeader.IDE = CAN_ID_STD;

		uint32_t mailBoxNum = 0;

		/*	for (uint8_t i = 0; i < 8; i++) {
		 msgData[i] = j++;
		 }
		 */

		*(float*) msgData = TiltX;
		*(float*) &msgData[4] = TiltY;

		HAL_CAN_AddTxMessage(hcan, &msgHeader, msgData, &mailBoxNum);
		N = HAL_CAN_GetTxMailboxesFreeLevel(hcan);

		msgHeader.StdId = ThisAddr + 1;

		for (uint8_t i = 0; i < 8; i++) {
			msgData[i] = j++;
		}

		*(float*) msgData = GyroYfl;
		*(uint16_t*) &msgData[4] = (uint16_t)(KGyro*1000);

		HAL_CAN_AddTxMessage(hcan, &msgHeader, msgData, &mailBoxNum);

		N = HAL_CAN_GetTxMailboxesFreeLevel(hcan);

		asm("Nop");

	}

}

void ProcessCANReceivedPacket(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef msgHeader;
	uint8_t msgData[8];

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &msgHeader, msgData);

	if (msgHeader.StdId == SET_ADDR_CAN) {
		switch (msgData[7]) {
		case 3:
			FlashReadAll();
			ThisAddr = (msgData[1] << 8) + msgData[0];
			FlashWriteAll();
			NVIC_SystemReset();
			break;

		case 4:
			FlashReadAll();
			KGyro = (float)((uint16_t)((msgData[1] << 8) + msgData[0]));
			KGyro=KGyro/1000;
			KAcc=1-KGyro;
			FlashWriteAll();
			FlashReadAll();
			NVIC_SystemReset();
			break;
		}

	} else {
		switch (msgData[0]) {
		case CALIBRATE:
			Calibrate = 1;
			LedStateMachine = 4;
			break;

		}
	}

	asm("Nop");

}
