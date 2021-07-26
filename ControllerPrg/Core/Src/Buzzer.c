#include "main.h"

void SetBuzzerFRQ(uint16_t Frq, uint16_t BuzzerTimer) {
	uint16_t Period;

	if (Frq > 54)
		Period = 3600000 / Frq;
	else
		Period = 0xffff;

	htim4.Instance->ARR = Period;
	if (Frq)
		htim4.Instance->CCR4 = Period / 2;
	else
		htim4.Instance->CCR4 = 0;

	BuzzerCnt = BuzzerTimer;

}

void BuzzerOff(void) {
	htim4.Instance->CCR4 = 0;
}
