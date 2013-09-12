/*
 * Angles.cpp
 *
 *  Created on: 12.09.2013
 *      Author: felix
 */

#include "Angles.h"

#include <math.h>

float radianToDegree(float radian) {
	return radian * 180 / M_PI;
}

float degreeToRadian(float degree) {
	return degree * M_PI / 180;
}
