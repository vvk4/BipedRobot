#include "main.h"

void Led(void);

uint16_t CntLed;
uint8_t LedStateMachine,CntFlashes;


void Led(void)
{

	  switch (LedStateMachine)
	  {
	  case 1:
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_SET);
		  CntLed=0;
		  LedStateMachine=2;
		  break;
	  case 2:
		  CntLed++;
		  if (CntLed>50)
		  {
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_RESET);
		  CntLed=0;
		  LedStateMachine=3;
		  }
		  break;
	  case 3:
		  CntLed++;
		  if (CntLed>2000)
		  {
		  CntLed=0;
		  LedStateMachine=1;
		  }
		  break;


	  case 4:		//flash 2 times
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_SET);
		  CntLed=0;
		  CntFlashes=2;
		  LedStateMachine=5;
		  break;
	  case 5:
		  CntLed++;
		  if (CntLed>50)
		  {
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_RESET);
		  CntLed=0;
		  LedStateMachine=6;
		  }
		  break;
	  case 6:
		  CntLed++;
		  if (CntLed>200)
		  {
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_SET);
		  CntLed=0;
		  CntFlashes--;
		  if (!CntFlashes)
		  LedStateMachine=4;
		  else
		  LedStateMachine=1;
		  }
		  break;

	  case 7:
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_SET);
		  CntLed=0;
		  LedStateMachine=8;
		  break;
	  case 8:
		  CntLed++;
		  if (CntLed>1000)
		  {
		  HAL_GPIO_WritePin(GreenLed, GPIO_PIN_RESET);
		  CntLed=0;
		  LedStateMachine=9;
		  }
		  break;
	  case 9:
		  CntLed++;
		  if (CntLed>1000)
		  {
		  CntLed=0;
		  LedStateMachine=1;
		  }
		  break;





	  default:
		  LedStateMachine=1;
		  break;

	  }

}


