/*
 * HealthOrb.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Felix
 */

#include "HealthOrb.h"

HealthOrb::HealthOrb() :
		Item(Vector2i(32, 32), "health_orb.png") {

}

std::string
HealthOrb::getName() const {
	return "Health Orb";
}

