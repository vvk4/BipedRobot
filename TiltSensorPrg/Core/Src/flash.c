#include "main.h"


void FlashReadAll(void);
void FlashWriteAll(void);

void FlashReadAll(void)
{
	AccXZero=*(__IO int32_t*)(START_ADDR_USER_PAGE + FLASH_ADDR_AccXZero);
	AccYZero=*(__IO int32_t*)(START_ADDR_USER_PAGE + FLASH_ADDR_AccYZero);
	AccZZero=*(__IO int32_t*)(START_ADDR_USER_PAGE + FLASH_ADDR_AccZZero);
	GyroXZero=*(__IO int32_t*)(START_ADDR_USER_PAGE + FLASH_ADDR_GyroXZero);
	GyroYZero=*(__IO int32_t*)(START_ADDR_USER_PAGE + FLASH_ADDR_GyroYZero);
	GyroZZero=*(__IO int32_t*)(START_ADDR_USER_PAGE + FLASH_ADDR_GyroZZero);
	ThisAddr=*(__IO uint32_t*)(START_ADDR_USER_PAGE+FLASH_ADDR_ThisAddr);
	KGyro=*(__IO float*)(START_ADDR_USER_PAGE+FLASH_ADDR_KGyro);

	KAcc=1-KGyro;


}


void FlashWriteAll(void)
{
FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t PageError = 0x00;
HAL_StatusTypeDef    flash_ok = HAL_ERROR;


    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = START_ADDR_USER_PAGE ;
    EraseInitStruct.NbPages     = 0x01;

    HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);


    while(flash_ok != HAL_OK)
          flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_AccXZero, (uint64_t)AccXZero);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_AccYZero, (uint64_t)AccYZero);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_AccZZero, (uint64_t)AccZZero);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_GyroXZero, (uint64_t)GyroXZero);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_GyroYZero, (uint64_t)GyroYZero);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_GyroZZero, (uint64_t)GyroZZero);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_ThisAddr, (uint64_t)ThisAddr);

    flash_ok = HAL_ERROR;
    while(flash_ok != HAL_OK)
        flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, START_ADDR_USER_PAGE + FLASH_ADDR_KGyro, (uint64_t)(*((uint32_t *)&KGyro)));

    HAL_FLASH_Lock();


}









/*

       uint32_t D1 = 0x11AEEDEE;//32 bit word
               uint32_t D2 = 0x14AEEDEE;//32 bit word


uint32_t rd=0;

uint32_t *ptr=	(START_ADDR_USER_PAGE + 0x4);
			rd=*ptr;//read_byte((uint32_t*)(START_ADDR_USER_PAGE + 0x4));

			*/
