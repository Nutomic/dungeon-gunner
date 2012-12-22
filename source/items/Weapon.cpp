/*
 * Weapon.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Weapon.h"

#include <algorithm>

#include <thor/Vectors.hpp>

#include "../util/Collection.h"
#include "../effects/Bullet.h"

const String Weapon::KEY_BULLET = "bullet";
const String Weapon::DEFAULT_BULLET = "bullet.yaml";
const String Weapon::KEY_INTERVAL = "interval";
const int Weapon::DEFAULT_INTERVAL = 250;

Weapon::Weapon(const Instances& instances, Body& holder, const Yaml& config) :
		Emitter(instances.collection),
		mHolder(holder),
		mWorld(instances.world),
		mBullet(config.get(KEY_BULLET, DEFAULT_BULLET)),
		mTimer(sf::milliseconds(config.get(KEY_INTERVAL, DEFAULT_INTERVAL))) {
	Vector2f holderSize = mHolder.getSize();
	Yaml bullet(mBullet);
	Vector2i bulletSize = bullet.get(Body::KEY_SIZE, Vector2i());
	mOffset = Vector2f(0,
			std::max(holderSize.x, holderSize.y) / 2 +
			std::max(bulletSize.x, bulletSize.y) / 2);
}

/**
 * Pull the trigger.
 */
void
Weapon::fire() {
	// Only call if has ammo, consider firing rate etc.
	if (mTimer.isExpired()) {
		emit();
		mTimer.start();
	}
}

std::shared_ptr<Particle>
Weapon::createParticle() {
	// Minus to account for positive y-axis going downwards in SFML.
	Vector2f offset(- mOffset);
	thor::rotate(offset, mHolder.getAngle());
	return std::shared_ptr<Particle>(new Bullet(mHolder.getPosition() + offset, mWorld,
			mHolder, mHolder.getAngle(), Yaml(mBullet)));
}
