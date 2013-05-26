/*
 * Weapon.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Weapon.h"

#include <Thor/Vectors.hpp>

#include "../World.h"
#include "../effects/Bullet.h"
#include "../util/Yaml.h"

Weapon::Weapon(World& world, Character& holder, const Yaml& config) :
		Emitter(world),
		mHolder(holder),
		mBullet(config.get(YAML_KEY::BULLET, YAML_DEFAULT::BULLET)),
		mLastShotWaitInterval(0),
		mFireInterval(config.get(YAML_KEY::INTERVAL, YAML_DEFAULT::INTERVAL)),
		mFire(false),
		mAutomatic(config.get(YAML_KEY::AUTOMATIC, YAML_DEFAULT::AUTOMATIC)) {
}

/**
 * Pull the trigger.
 */
void
Weapon::pullTrigger() {
	mFire = true;
}

/**
 * Release the trigger.
 */
void
Weapon::releaseTrigger() {
	mFire = false;
}

/**
 * Fire if the trigger has been pulled, time between bullets is over, has ammo etc.
 *
 * @param elapsed Amount of time to simulate.
 */
void
Weapon::onThink(int elapsed) {
	// Waiting for next shot, subtract time since last onThink.
	if (mLastShotWaitInterval > 0)
		mLastShotWaitInterval -= elapsed;
	// Only reset to zero if we didn't recently fire (allow catching up for missed bullets).
	else
		mLastShotWaitInterval = 0;
	// Loop just in case we miss a bullet to fire.
	while (mFire && mLastShotWaitInterval <= 0) {
		mLastShotWaitInterval += mFireInterval;
		emit();
		if (!mAutomatic)
			mFire = false;
	}
}

std::shared_ptr<Sprite>
Weapon::createParticle() {
	// Minus to account for positive y-axis going downwards in SFML.
	sf::Vector2f offset(0, - mHolder.getRadius());
	thor::rotate(offset, thor::polarAngle(mHolder.getDirection()));
	return std::shared_ptr<Sprite>(new Bullet(mHolder.getPosition() + offset,
			mHolder, mHolder.getDirection(), Yaml(mBullet)));
}
