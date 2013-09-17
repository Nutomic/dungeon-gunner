/*
 * HealthOrb.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Felix
 */

#include "HealthOrb.h"

const std::string HealthOrb::CONFIG_NAME = "health_orb.yaml";

HealthOrb::HealthOrb() :
		HealthOrb(Yaml(CONFIG_NAME)) {
}

HealthOrb::HealthOrb(const Yaml& config) :
		Item(config.get("size", Vector2i()), config.get("texture", std::string())),
		mName(config.get("name", std::string())),
		mAmountHealed(config.get("amount_healed", 0)) {
}

int
HealthOrb::getAmountHealed() const {
	return mAmountHealed;
}

std::string
HealthOrb::getName() const {
	return mName;
}
