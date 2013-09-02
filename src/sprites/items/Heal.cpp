/*
 * Heal.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Heal.h"

#include "../abstract/Character.h"

const Yaml Heal::CONFIG("res/yaml/heal.yaml");

Heal::Heal() :
		Gadget(CONFIG.get("name", std::string()), CONFIG.get("cooldown", 0)),
		mHealedTotal(CONFIG.get("amount_healed", 0)),
		mTimePerPoint(sf::milliseconds(CONFIG.get("time_healing", 0))),
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

