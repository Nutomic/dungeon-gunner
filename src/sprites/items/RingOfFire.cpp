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

const std::string RingOfFire::CONFIG_NAME = "ring_of_fire.yaml";

RingOfFire::RingOfFire(World& world) :
		RingOfFire(world, Yaml(CONFIG_NAME)) {
}

RingOfFire::RingOfFire(World& world, const Yaml& config) :
	Gadget(config.get("name", std::string()), config.get("cooldown", 0)),
	mWavesPerUse(config.get("waves_per_use", 0)),
	mAngleOffset(config.get("angle_offset", 0.0f)),
	mBulletsPerWave(config.get("bullets_per_wave", 0)),
	mDelay(sf::milliseconds(config.get("delay", 0))),
	mCurrentWave(mWavesPerUse + 1),
	mWorld(world),
	mBullet(config.get("bullet", std::string())) {

}

void
RingOfFire::onUse(Character& character) {
	mCharacter = &character;
	mCurrentWave = 0;
}

void
RingOfFire::onThink(int elapsed) {
	if (mCurrentWave < mWavesPerUse && mTimer.isExpired()) {
		for (int angle = mCurrentWave * 10; angle <= 360; angle += 360 / mBulletsPerWave) {
			Vector2f direction(thor::rotatedVector(mCharacter->getDirectionVector(), (float) angle) *
					mCharacter->getRadius());

			std::shared_ptr<Sprite> projectile(new Bullet(mCharacter->getPosition() + direction,
					*mCharacter, direction, mBullet, 200, 20, 0));
			mWorld.insert(projectile);
		}

		mTimer.restart(mDelay);
		mCurrentWave++;
	}
}

Gadget::GadgetType
RingOfFire::getType() const {
	return GadgetType::RINGOFFIRE;
}

