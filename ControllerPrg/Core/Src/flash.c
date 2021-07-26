#include "main.h"

#define START_ADDR_USER_PAGE 0x080E0000
#define SIZE_OF_OPTIONS_ARRAY 500

uint8_t OptionsArray[SIZE_OF_OPTIONS_ARRAY];
uint16_t CntOptions;

void FlashWriteAll(void) {
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError = 0x00;
	uint8_t i;
	HAL_StatusTypeDef flash_ok;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.NbSectors = 1;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = 11;

	__HAL_FLASH_CLEAR_FLAG(
			FLASH_SR_PGPERR | FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PGAERR);

	flash_ok = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

	for (i = 0; i < (SIZE_OF_OPTIONS_ARRAY / 4); i++) {
		flash_ok = HAL_ERROR;
		while (flash_ok != HAL_OK)
			flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
			START_ADDR_USER_PAGE + i * 4,
					(uint64_t) *(uint32_t*) &OptionsArray[i * 4]);
	}

	HAL_FLASH_Lock();

}

void FlashReadAll(void) {
	uint16_t i;
	for (i = 0; i < (SIZE_OF_OPTIONS_ARRAY / 4); i++) {
		*(uint32_t*) &OptionsArray[i * 4] =
				*(__IO uint32_t*) (START_ADDR_USER_PAGE + i * 4);
	}

//	ThisAddr=*(__IO uint32_t*)(START_ADDR_USER_PAGE+FLASH_ADDR_ThisAddr);
	//KGyro=*(__IO float*)(START_ADDR_USER_PAGE+FLASH_ADDR_KGyro);

}

void OptionsArrayAddFloat(float Fl) {
	*(float*) &OptionsArray[CntOptions] = Fl;
	CntOptions = CntOptions + sizeof(float);
}

float OptionsArrayGetFloat(void) {
	float Fl = *(float*) &OptionsArray[CntOptions];
	CntOptions = CntOptions + sizeof(float);
	return Fl;
}

void OptionsToArray(void) {
	CntOptions = 4;

	OptionsArray[0] = 0xff;
	OptionsArray[1] = 0xff;

	OptionsArray[3] = TRM_OPTIONS_PACKET;

	OptionsArrayAddFloat((float) 1234.5678);
	OptionsArrayAddFloat((float) 5678.9);

	OptionsArray[2] = CntOptions - 3;

	OptionsArray[CntOptions] = CalcCheckSumm(OptionsArray[2] + 1,
			&OptionsArray[2]);
}

void ArrayToOptions(void) {
	float Tmp1, Tmp2;
	CntOptions = 4;
	Tmp1 = OptionsArrayGetFloat();
	Tmp2 = OptionsArrayGetFloat();
	asm("Nop");
}

