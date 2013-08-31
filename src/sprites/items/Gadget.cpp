/*
 * Gadget.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Gadget.h"

#include "../abstract/Character.h"
#include "Heal.h"
#include "Shield.h"
#include "RingOfFire.h"

Gadget::Gadget(std::string name) :
		Item(sf::Vector2f(32, 32), "item.png"),
		mName(name) {
}

/**
 * Constructs a new gadget of type and returns a pointer to it.
 */
std::shared_ptr<Gadget>
Gadget::getGadget(World& world, GadgetType type) {
	switch (type) {
	case SHIELD:
		return std::shared_ptr<Gadget>(new Shield());
	case HEAL:
		return std::shared_ptr<Gadget>(new Heal());
	case RINGOFFIRE:
		return std::shared_ptr<Gadget>(new RingOfFire(world));
	default:
		return std::shared_ptr<Gadget>();
	}
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
