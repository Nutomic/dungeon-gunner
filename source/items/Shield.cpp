/*
 * Shield.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Shield.h"

#include "../abstract/Character.h"
#include "../sprites/RotatingShield.h"
#include "../World.h"

void
Shield::onUse(Character& character) {
	mCharacter = &character;
	if (mRotatingShield)
		mRotatingShield->setDelete(true);
	sf::Vector2f offset = mCharacter->getDirection() * mCharacter->getRadius();
	mRotatingShield = std::shared_ptr<RotatingShield>(
			new RotatingShield(mCharacter->getPosition() + offset));
	mCharacter->mWorld.insert(mRotatingShield);
}

void
Shield::onThink(int elapsed) {
	if (mRotatingShield) {
		mRotatingShield->setDirection(mCharacter->getPosition() -
				mRotatingShield->getPosition());
	}
}

sf::Time
Shield::getCooldownTime() {
	return sf::seconds(10);
}
