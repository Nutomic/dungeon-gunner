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

Weapon::Weapon(const Instances& instances, Physical& holder, const Yaml& config) :
		Emitter(instances.collection),
		mHolder(holder),
		mWorld(instances.world),
		mOffset(0, std::max(mHolder.getSize().x, mHolder.getSize().y) / 2 +
			b2_linearSlop +
			std::max(Bullet::SIZE.x, Bullet::SIZE.y) / 2),
		mBullet(config.get<String>(KEY_BULLET)) {
}

/**
 * Pull the trigger.
 */
void
Weapon::fire() {
	// Only call if has ammo, consider firing rate etc.
	emit();
}

std::shared_ptr<Particle>
Weapon::createParticle() {
	// Minus to account for positive y-axis going downwards in SFML.
	Vector2f offset(- mOffset);
	thor::rotate(offset, mHolder.getAngle());
	return std::shared_ptr<Particle>(new Bullet(mHolder.getPosition() + offset, 	mWorld,
			mHolder, mHolder.getAngle(), Yaml(mBullet)));
}
