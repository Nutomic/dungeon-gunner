/*
 * Angles.cpp
 *
 *  Created on: 12.09.2013
 *      Author: felix
 */

#include "Angles.h"

#include <LTBL/Utils.h>

float radianToDegree(float radian) {
	return radian * 180 / ltbl::pi;
}

float degreeToRadian(float degree) {
	return degree * ltbl::pi / 180;
}
