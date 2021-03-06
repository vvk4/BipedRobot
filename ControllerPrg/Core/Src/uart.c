#include "main.h"

uint16_t CntRxTimeOut, CntRec_Rc, CntPacket;
uint8_t GettingPacketRx_FL, BtPrev_Rc, RecArray[SIZE_OF_RECARRAY],
		PacketReceived_FL, ConfirmCMD;

void ProcessUARTCmd(void);

void ReceivePacketUART(UART_HandleTypeDef *huart) {
	uint8_t Bt;

	if (huart->Instance->SR & USART_SR_RXNE) {
		Bt = (uint8_t) huart->Instance->DR;

		CntRxTimeOut = 500;

		if (!GettingPacketRx_FL) {
			if ((BtPrev_Rc == 0xff) && (Bt == 0xff)) {
				BtPrev_Rc = 0;
				GettingPacketRx_FL = 1;
				CntRec_Rc = 0;
			} else {
				BtPrev_Rc = Bt;
			}
		} else {
			if (CntRec_Rc > (SIZE_OF_RECARRAY - 1))
				GettingPacketRx_FL = 0;
			else {
				RecArray[CntRec_Rc] = Bt;
				CntRec_Rc++;

				if (CntRec_Rc > (RecArray[0] + 1)) {
					GettingPacketRx_FL = 0;

					if (CalcCheckSumm(RecArray[0] + 1, RecArray)
							!= RecArray[RecArray[0] + 1]) {
						asm("Nop");

					} else {
						PacketReceived_FL = 1;
						ProcessUARTCmd();
					}
				}
			}
		}
	}
}

void ProcessUARTCmd(void) {
	uint8_t Cmd = RecArray[1];
	uint8_t Byte;
	int16_t PWM;

	if (!PacketReceived_FL)
		return;

	PacketReceived_FL = 0;

	CntPacket++;

	ConfirmCMD = 1;

	switch (Cmd) {
	case SET_ADDR:
		RecArray[9] = 3;
		Can_TxMessage(&hcan2, 0, SET_ADDR_CAN, 8, &RecArray[2]);
		break;
	case CALIBR1:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_ADDR, 1, &Byte);
		break;
	case CALIBR2:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_HIP_L_ADDR, 1, &Byte);
		break;
	case CALIBR3:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_LOWERLEG_L_ADDR, 1, &Byte);
		break;
	case CALIBR4:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_FOOT_L_ADDR, 1, &Byte);
		break;
	case CALIBR5:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_HIP_R_ADDR, 1, &Byte);
		break;
	case CALIBR6:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_LOWERLEG_R_ADDR, 1, &Byte);
		break;
	case CALIBR7:
		Byte = 1;
		Can_TxMessage(&hcan2, 0, 0x100 + TILT_SENSOR_FOOT_R_ADDR, 1, &Byte);
		break;
	case SET_KGYRO:
		RecArray[9] = 4;
		Can_TxMessage(&hcan2, 0, SET_ADDR_CAN, 8, &RecArray[2]);
		break;
	case SET_PWM_Motor_L1:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_L1);
		break;
	case SET_PWM_Motor_L2:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_L2);
		break;
	case SET_PWM_Motor_L3:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_L3);
		break;
	case SET_PWM_Motor_L4:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_L4);
		break;
	case SET_PWM_Motor_R1:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_R1);
		break;
	case SET_PWM_Motor_R2:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_R2);
		break;
	case SET_PWM_Motor_R3:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_R3);
		break;
	case SET_PWM_Motor_R4:
		PWM = (RecArray[3] << 8) + RecArray[2];
		SetMotorPWM(PWM, MOTOR_R4);
		break;
	case GET_OPTIONS:
		FlashReadAll();
		ArrayToOptions();
		SetBuzzerFRQ(1000, 50);
		break;
	case WRITE_OPTIONS:
		OptionsToArray();
		FlashWriteAll();
		SetBuzzerFRQ(1000, 500);
		break;
	case CLEAR_ALL:
		CntSamples=0;
		break;

	}
}

void TrmDataPacketUART(UART_HandleTypeDef *huart) {
	uint16_t Cnt = 4;

	UART2TransmittArray[0] = 0xff;
	UART2TransmittArray[1] = 0xff;
	UART2TransmittArray[3] = TRM_DATA_PACKET; //CMD

	*(uint32_t*) &UART2TransmittArray[Cnt] = CntSamples;
	Cnt = Cnt + sizeof(uint32_t);

	*(float*) &UART2TransmittArray[Cnt] = Tilt1X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt1Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro1;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = Tilt2X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt2Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro2;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = Tilt3X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt3Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro3;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = Tilt4X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt4Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro4;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = Tilt5X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt5Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro5;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = Tilt6X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt6Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro6;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = Tilt7X;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Tilt7Y;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = Gyro7;
	Cnt = Cnt + sizeof(float);

	*(uint16_t*) &UART2TransmittArray[Cnt] = KGyro1;
	Cnt = Cnt + sizeof(uint16_t);

	*(float*) &UART2TransmittArray[Cnt] = GyroHipJointR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroHipJointL;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroHipR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroHipL;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroLowerlegR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroLowerlegL;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroFootR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = GyroFootL;
	Cnt = Cnt + sizeof(float);

	*(float*) &UART2TransmittArray[Cnt] = YHipJointR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = YHipJointL;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = XHipR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = XHipL;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = XLowerLegR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = XLowerLegL;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = XFootR;
	Cnt = Cnt + sizeof(float);
	*(float*) &UART2TransmittArray[Cnt] = XFootL;
	Cnt = Cnt + sizeof(float);



	UART2TransmittArray[2] = Cnt - 3;

	UART2TransmittArray[Cnt] = CalcCheckSumm(UART2TransmittArray[2] + 1,
			&UART2TransmittArray[2]);

	HAL_UART_Transmit_DMA(huart, (uint8_t*) UART2TransmittArray,
			UART2TransmittArray[2] + 4);

}

void TrmCfmPacketUART(UART_HandleTypeDef *huart) {
	uint16_t Cnt = 4;

	UART2TransmittArray[0] = 0xff;
	UART2TransmittArray[1] = 0xff;
	UART2TransmittArray[3] = TRM_CFM_PACKET; //CMD

	UART2TransmittArray[2] = Cnt - 3;

	UART2TransmittArray[Cnt] = CalcCheckSumm(UART2TransmittArray[2] + 1,
			&UART2TransmittArray[2]);

	HAL_UART_Transmit_DMA(huart, (uint8_t*) UART2TransmittArray,
			UART2TransmittArray[2] + 4);

}

