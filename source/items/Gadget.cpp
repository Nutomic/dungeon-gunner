/*
 * Gadget.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Gadget.h"

#include "../abstract/Character.h"

Gadget::Gadget(std::string name) :
		Item(sf::Vector2f(40, 40), "gadget.png"),
		mName(name) {
}

void
Gadget::use(Character& character) {
	if (mCooldownTimer.isExpired()) {
		onUse(character);
		mCooldownTimer.restart(getCooldownTime());
	}
}

std::string
Gadget::getName() const {
	return mName;
}
