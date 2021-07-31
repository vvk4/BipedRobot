#include "main.h"
#include <math.h>

float LHip = 350, LLowerLeg = 353;
float BC, HLeg, Vbok;
float TargetRHipJoint, TargetRLowerLeg, TargetRHip, TargetRFoot, TargetRBottom;



void PWM_Off(void) {
	htim2.Instance->CCR1 = htim2.Instance->CCR2 = htim2.Instance->CCR3 =
			htim2.Instance->CCR4 = 0;
	htim3.Instance->CCR1 = htim3.Instance->CCR2 = htim3.Instance->CCR3 =
			htim3.Instance->CCR4 = 0;
}

void SetMotorPWM(int16_t PWM, uint16_t MotorNum) {

	if (PWM > MAX_PWM)
		PWM = MAX_PWM;
	if (PWM < -MAX_PWM)
		PWM = -MAX_PWM;

	switch (MotorNum) {
	case MOTOR_L1:
		if (PWM >= 0) {
			htim2.Instance->CCR1 = PWM;
			htim2.Instance->CCR2 = 0;
		} else {
			htim2.Instance->CCR1 = 0;
			htim2.Instance->CCR2 = -PWM;
		}
		break;
	case MOTOR_L2:
		if (PWM >= 0) {
			htim2.Instance->CCR3 = PWM;
			htim2.Instance->CCR4 = 0;
		} else {
			htim2.Instance->CCR3 = 0;
			htim2.Instance->CCR4 = -PWM;
		}
		break;
	case MOTOR_L3:
		if (PWM >= 0) {
			htim3.Instance->CCR1 = PWM;
			htim3.Instance->CCR2 = 0;
		} else {
			htim3.Instance->CCR1 = 0;
			htim3.Instance->CCR2 = -PWM;
		}
		break;
	case MOTOR_L4:
		if (PWM >= 0) {
			htim3.Instance->CCR3 = PWM;
			htim3.Instance->CCR4 = 0;
		} else {
			htim3.Instance->CCR3 = 0;
			htim3.Instance->CCR4 = -PWM;
		}
		break;
	case MOTOR_R1:
		PWM = PWM << 1;
		if (PWM >= 0) {
			htim8.Instance->CCR3 = PWM;
			htim8.Instance->CCR4 = 0;
		} else {
			htim8.Instance->CCR3 = 0;
			htim8.Instance->CCR4 = -PWM;
		}
		break;
	case MOTOR_R2:
		PWM = PWM << 1;
		if (PWM >= 0) {
			htim8.Instance->CCR1 = PWM;
			htim8.Instance->CCR2 = 0;
		} else {
			htim8.Instance->CCR1 = 0;
			htim8.Instance->CCR2 = -PWM;
		}
		break;
	case MOTOR_R3:
		PWM = PWM << 1;
		if (PWM >= 0) {
			htim1.Instance->CCR3 = PWM;
			htim1.Instance->CCR4 = 0;
		} else {
			htim1.Instance->CCR3 = 0;
			htim1.Instance->CCR4 = -PWM;
		}
		break;
	case MOTOR_R4:
		PWM = PWM << 1;
		if (PWM >= 0) {
			htim1.Instance->CCR1 = PWM;
			htim1.Instance->CCR2 = 0;
		} else {
			htim1.Instance->CCR1 = 0;
			htim1.Instance->CCR2 = -PWM;
		}
		break;

	}
}


void CalculateAngles(void) {

	float BD, Alfa1, Alfa2, HLegTemp;

	BC = -80;
	HLeg = 530;
	Vbok = 120;

	HLegTemp = HLeg;

	HLeg = HLeg * HLeg + Vbok * Vbok;
	HLeg = sqrt(HLeg);

	TargetRHipJoint = HLegTemp / HLeg;
	if (Vbok > 0) {
		TargetRHipJoint = acos(TargetRHipJoint) * 57.296;
		TargetRBottom = TargetRHipJoint;
		TargetRHipJoint = TargetRHipJoint + 90;
	} else {
		TargetRHipJoint = acos(TargetRHipJoint) * 57.296;
		TargetRBottom = -TargetRHipJoint;
		TargetRHipJoint = 90 - TargetRHipJoint;
	}

	BD = (float) BC * (float) BC + (float) HLeg * (float) HLeg;
	BD = sqrt(BD);
	Alfa1 = HLeg / BD;
	Alfa1 = acos(Alfa1) * 57.296;

	Alfa2 = ( LLowerLeg *  LLowerLeg + BD * BD
			-  LHip *  LHip)
			/ ((float) 2 *  LLowerLeg * BD);
	Alfa2 = acos(Alfa2) * 57.296;

	TargetRLowerLeg = ( LLowerLeg *  LLowerLeg
			+  LHip *  LHip - BD * BD)
			/ ((float) 2 *  LLowerLeg *  LHip);
	TargetRLowerLeg = acos(TargetRLowerLeg) * 57.296;

	if (BC < 0)
		TargetRHip = 180 - (180 - TargetRLowerLeg - Alfa2 + (90 - Alfa1)) + 90;
	else
		TargetRHip = 180 - Alfa1 - (180 - TargetRLowerLeg - Alfa2);

	if (BC < 0)
		TargetRFoot = 90 - Alfa1 - Alfa2;
	else
		TargetRFoot = 90 + Alfa1 - Alfa2;

	if ((TargetRFoot - (int) TargetRFoot) >= 0.5) {
		TargetRFoot = (int) TargetRFoot + 1;
	}

	if ((TargetRLowerLeg - (int) TargetRLowerLeg) >= 0.5) {
		TargetRLowerLeg = (int) TargetRLowerLeg + 1;
	}

	if ((TargetRHip - (int) TargetRHip) >= 0.5) {
		TargetRHip = (int) TargetRHip + 1;
	}

	TargetRHip = TargetRHip - 90;

	HLeg = HLegTemp;

}
