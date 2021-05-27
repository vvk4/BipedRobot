/*
 * TiltCalculating.c
 *
 *  Created on: Feb 13, 2021
 *      Author: Viacheslav Kulakov mailto: vvk43210@yandex.ru
 */

#include "main.h"
#include <math.h>

void CalcullateTilts(void);



float XNew, YNew, ZNew;
float AccXFlPrev, AccYFlPrev, AccZFlPrev;
float KGyro = 0.998, KAcc;
float AccXFlPrev, AccYFlPrev, AccZFlPrev;
float TiltX, TiltY, TiltXPrev, TiltYPrev;
_Bool Starting = 1;

void CalcullateTilts(void) {

	float RotX = GyroXfl * TICK_PERIOD;
	float RotY = GyroYfl * TICK_PERIOD;
	float RotZ = GyroZfl * TICK_PERIOD;

	float Alf = RotX * 8.75 / 57296, Bt = RotY * 8.75 / 57296, Gm = RotZ * 8.75
			/ 57296;
	float SinBt = sin(Bt);
	float CosBt = cos(Bt);
	float SinAlf = sin(Alf);
	float CosAlf = cos(Alf);
	float SinGm = sin(Gm);
	float CosGm = cos(Gm);

	KAcc = 1 - KGyro;

	if (!Starting) {

		XNew = (AccXFlPrev * CosBt * CosGm - AccYFlPrev * CosBt * SinGm
				+ AccZFlPrev * SinBt) * KGyro + KAcc * AccXfl;
		YNew = (AccXFlPrev * SinAlf * SinBt * CosGm
				+ AccXFlPrev * CosAlf * SinGm
				+ AccYFlPrev * SinAlf * SinBt * SinGm
				+ AccYFlPrev * CosAlf * CosGm - AccZFlPrev * SinAlf * CosBt)
				* KGyro + KAcc * AccYfl;
		ZNew = (-AccXFlPrev * CosAlf * SinBt * CosGm
				+ AccXFlPrev * SinAlf * SinGm
				+ AccYFlPrev * CosAlf * SinBt * SinGm
				+ AccYFlPrev * SinAlf * CosGm + AccZFlPrev * CosAlf * CosBt)
				* KGyro + KAcc * AccZfl;

		if ((YNew == 0) && (ZNew == 0))
			TiltX = TiltXPrev;
		else
			TiltX = atan(XNew / (sqrt(YNew * YNew + ZNew * ZNew))) * 57.296;
		TiltXPrev = TiltX;

		if ((XNew == 0) && (ZNew == 0))
			TiltY = TiltYPrev;
		else
			TiltY = atan(YNew / (sqrt(XNew * XNew + ZNew * ZNew))) * 57.296;
		TiltYPrev = TiltY;

		AccXFlPrev = XNew;
		AccYFlPrev = YNew;
		AccZFlPrev = ZNew;

	} else {
		AccXFlPrev = AccXfl;
		AccYFlPrev = AccYfl;
		AccZFlPrev = AccZfl;
		Starting=0;
	}





}

