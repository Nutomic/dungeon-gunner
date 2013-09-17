/*
 * Heal.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Heal.h"

#include "../abstract/Character.h"

const std::string Heal::CONFIG_NAME = "heal.yaml";

Heal::Heal() :
		Heal(Yaml(CONFIG_NAME)) {
}

Heal::Heal(const Yaml& config) :
		Gadget(config.get("name", std::string()), config.get("cooldown", 0)),
		mHealedTotal(config.get("amount_healed", 0)),
		mTimePerPoint(sf::milliseconds(config.get("time_healing", 0))),
		mHealed(mHealedTotal + 1) {
}

void
Heal::onUse(Character& character) {
	mCharacter = &character;
	mHealed = 0;
}

void
Heal::onThink(int elapsed) {
	if (mHealed < mHealedTotal && mTimer.isExpired()) {
		mCharacter->onDamage(-1);
		mHealed++;
		mTimer.restart(mTimePerPoint);
	}
}

Gadget::GadgetType
Heal::getType() const {
	return GadgetType::HEAL;
}

