#include "main.h"



void SetMotorPWM(int16_t PWM,uint16_t MotorNum);
void PWM_Off(void);



void PWM_Off(void)
{
htim2.Instance->CCR1=htim2.Instance->CCR2=htim2.Instance->CCR3=htim2.Instance->CCR4=0;
htim3.Instance->CCR1=htim3.Instance->CCR2=htim3.Instance->CCR3=htim3.Instance->CCR4=0;
}




void SetMotorPWM(int16_t PWM,uint16_t MotorNum)
{

	if (PWM>MAX_PWM)
		PWM=MAX_PWM;
	if (PWM<-MAX_PWM)
		PWM=-MAX_PWM;


	switch (MotorNum)
	{
	case MOTOR_L1:
		if (PWM>=0)
		{
			htim2.Instance->CCR1=PWM;
			htim2.Instance->CCR2=0;
		}
		else
		{
			htim2.Instance->CCR1=0;
			htim2.Instance->CCR2=-PWM;
		}
		break;
	case MOTOR_L2:
		if (PWM>=0)
		{
			htim2.Instance->CCR3=PWM;
			htim2.Instance->CCR4=0;
		}
		else
		{
			htim2.Instance->CCR3=0;
			htim2.Instance->CCR4=-PWM;
		}
		break;
	case MOTOR_L3:
		if (PWM>=0)
		{
			htim3.Instance->CCR1=PWM;
			htim3.Instance->CCR2=0;
		}
		else
		{
			htim3.Instance->CCR1=0;
			htim3.Instance->CCR2=-PWM;
		}
		break;
	case MOTOR_L4:
		if (PWM>=0)
		{
			htim3.Instance->CCR3=PWM;
			htim3.Instance->CCR4=0;
		}
		else
		{
			htim3.Instance->CCR3=0;
			htim3.Instance->CCR4=-PWM;
		}
		break;
	case MOTOR_R1:
		PWM=PWM<<1;
		if (PWM>=0)
		{
			htim8.Instance->CCR3=PWM;
			htim8.Instance->CCR4=0;
		}
		else
		{
			htim8.Instance->CCR3=0;
			htim8.Instance->CCR4=-PWM;
		}
		break;
	case MOTOR_R2:
		PWM=PWM<<1;
		if (PWM>=0)
		{
			htim8.Instance->CCR1=PWM;
			htim8.Instance->CCR2=0;
		}
		else
		{
			htim8.Instance->CCR1=0;
			htim8.Instance->CCR2=-PWM;
		}
		break;
	case MOTOR_R3:
		PWM=PWM<<1;
		if (PWM>=0)
		{
			htim1.Instance->CCR3=PWM;
			htim1.Instance->CCR4=0;
		}
		else
		{
			htim1.Instance->CCR3=0;
			htim1.Instance->CCR4=-PWM;
		}
		break;
	case MOTOR_R4:
		PWM=PWM<<1;
		if (PWM>=0)
		{
			htim1.Instance->CCR1=PWM;
			htim1.Instance->CCR2=0;
		}
		else
		{
			htim1.Instance->CCR1=0;
			htim1.Instance->CCR2=-PWM;
		}
		break;




	}
}
