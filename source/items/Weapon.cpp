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
const String Weapon::KEY_INTERVAL = "interval";

Weapon::Weapon(const Instances& instances, Body& holder, const Yaml& config) :
		Emitter(instances.collection),
		mHolder(holder),
		mWorld(instances.world),
		mBullet(config.get<String>(KEY_BULLET)),
		mTimer(sf::milliseconds(config.get<int>(KEY_INTERVAL))) {
	Yaml bullet(mBullet);
	Vector2i bulletSize = bullet.get<Vector2i>(Body::KEY_SIZE);
	mOffset = Vector2f(0, std::max(mHolder.getSize().x, mHolder.getSize().y) / 2 +
			b2_linearSlop +
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
