/*
 * HealthOrb.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Felix
 */

#include "HealthOrb.h"

const Yaml HealthOrb::CONFIG("res/yaml/health_orb.yaml");

HealthOrb::HealthOrb() :
		Item(CONFIG.get("size", Vector2i()), CONFIG.get("texture", std::string())),
		mName(CONFIG.get("name", std::string())),
		mAmountHealed(CONFIG.get("amount_healed", 0)) {
}

int
HealthOrb::getAmountHealed() const {
	return mAmountHealed;
}

std::string
HealthOrb::getName() const {
	return mName;
}
