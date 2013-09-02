/*
 * Shield.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Shield.h"

#include "../abstract/Character.h"
#include "../RotatingShield.h"
#include "../../World.h"

const Yaml Shield::CONFIG("res/yaml/rotating_shield.yaml");

Shield::Shield() :
		Gadget(CONFIG.get("name", std::string()), CONFIG.get("cooldown", 0)) {
}

void
Shield::onUse(Character& character) {
	mCharacter = &character;
	if (mRotatingShield)
		mRotatingShield->setDelete(true);
	Vector2f offset = mCharacter->getDirection() * mCharacter->getRadius();
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

Shield::GadgetType
Shield::getType() const {
	return GadgetType::SHIELD;
}
