/*
 * RingOfFire.cpp
 *
 *  Created on: 29.08.2013
 *      Author: Felix
 */

#include "RingOfFire.h"

#include <Thor/Vectors.hpp>

#include "../effects/Bullet.h"
#include "../../World.h"

RingOfFire::RingOfFire(World& world) :
		Gadget("Ring of Fire"),
		mWorld(world),
		mBullet("bullet.yaml") {
}

void
RingOfFire::onUse(Character& character) {
	mCharacter = &character;
	mCurrentWave = 0;
}

void
RingOfFire::onThink(int elapsed) {
	if (mCurrentWave < WAVES_PER_USE && mTimer.isExpired()) {
		for (int angle = mCurrentWave * 10; angle <= 360; angle += 30) {
			Vector2f direction(thor::rotatedVector(mCharacter->getDirection(), (float) angle) *
					mCharacter->getRadius());

			std::shared_ptr<Sprite> projectile(new Bullet(mCharacter->getPosition() + direction,
					*mCharacter, direction, mBullet, 200, 20, 0));
			mWorld.insert(projectile);
		}

		mTimer.restart(sf::milliseconds(500));
		mCurrentWave++;
	}
}

sf::Time
RingOfFire::getCooldownTime() const {
	return sf::seconds(5);
}

Gadget::GadgetType
RingOfFire::getType() const {
	return GadgetType::RINGOFFIRE;
}

