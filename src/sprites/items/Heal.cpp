/*
 * SlowHeal.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Heal.h"

#include "../abstract/Character.h"

Heal::Heal() :
		Gadget("Heal") {
}

void
Heal::onUse(Character& character) {
	mCharacter = &character;
	mHealedTotal = 0;
}

void
Heal::onThink(int elapsed) {
	if (mHealedTotal < 50 && mTimer.isExpired()) {
		mCharacter->onDamage(-1);
		mHealedTotal += 1;
		mTimer.restart(sf::milliseconds(75));
	}
}

sf::Time
Heal::getCooldownTime() {
	return sf::seconds(5);
}

