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
#include "../util/Loader.h"
#include "../util/ResourceManager.h"

const int Weapon::BULLET_DAMAGE = 10;

Weapon::Weapon(Physical& holder, Collection& collection, b2World& world,
	const Vector2i& holderSize) :
		Emitter(collection),
		mHolder(holder),
		mBulletTexture(ResourceManager::i()
                         .acquire(Loader::i().fromFile<sf::Texture>("bullet.png"))),
		mWorld(world),
		mOffset(0, std::max(holderSize.x, holderSize.y) / 2 +
			b2_linearSlop +
			std::max(Bullet::SIZE.x, Bullet::SIZE.y) / 2) {
}

Weapon::~Weapon() {
}

/**
 * Call on any button press/refire.
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
	return std::shared_ptr<Particle>(new Bullet(mHolder.getPosition() + offset,
			mWorld, mBulletTexture, mHolder, mHolder.getAngle(), BULLET_DAMAGE));
}
