/*
 * RotatingShield.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "RotatingShield.h"

#include "../util/Yaml.h"

RotatingShield::RotatingShield(const Vector2f& position) :
		Rectangle(position, CATEGORY_WORLD, MASK_ALL,
				Yaml("rotating_shield.yaml")) {
}
