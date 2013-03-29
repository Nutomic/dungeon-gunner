/*
 * Weapon.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Weapon.h"

#include <algorithm>

#include <thor/Vectors.hpp>

#include "../World.h"
#include "../effects/Bullet.h"
#include "../util/Yaml.h"

const std::string Weapon::KEY_BULLET = "bullet";
const std::string Weapon::DEFAULT_BULLET = "bullet.yaml";
const std::string Weapon::KEY_INTERVAL = "interval";
const int Weapon::DEFAULT_INTERVAL = 250;
const std::string Weapon::KEY_AUTOMATIC = "automatic";
const bool Weapon::DEFAULT_AUTOMATIC = false;

Weapon::Weapon(World& world, Sprite& holder, const Yaml& config) :
		Emitter(world),
		mHolder(holder),
		mBullet(config.get(KEY_BULLET, DEFAULT_BULLET)),
		mLastShotWaitInterval(0),
		mFireInterval(config.get(KEY_INTERVAL, DEFAULT_INTERVAL)),
		mFire(false),
		mAutomatic(config.get(KEY_AUTOMATIC, DEFAULT_AUTOMATIC)) {
	sf::Vector2f holderSize = mHolder.getSize();
	Yaml bullet(mBullet);
	sf::Vector2f bulletSize = bullet.get(Sprite::KEY_SIZE, sf::Vector2f());
	mOffset = sf::Vector2f(0,
			std::max(holderSize.x, holderSize.y) / 2 +
			std::max(bulletSize.x, bulletSize.y) / 2);
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
	if (mLastShotWaitInterval > 0) {
		mLastShotWaitInterval -= elapsed;
	}
	// Only reset to zero if we didn't recently fire (allow catching up for missed bullets).
	else {
		mLastShotWaitInterval = 0;
	}
	// Loop just in case we miss a bullet to fire.
	while (mFire && mLastShotWaitInterval <= 0) {
		mLastShotWaitInterval += mFireInterval;
		emit();
		if (!mAutomatic) {
			mFire = false;
		}
	}
}

std::shared_ptr<Sprite>
Weapon::createParticle() {
	// Minus to account for positive y-axis going downwards in SFML.
	sf::Vector2f offset(- mOffset);
	thor::rotate(offset, mHolder.getAngle());
	return std::shared_ptr<Sprite>(new Bullet(mHolder.getPosition() + offset,
			mHolder, mHolder.getAngle(), Yaml(mBullet)));
}
