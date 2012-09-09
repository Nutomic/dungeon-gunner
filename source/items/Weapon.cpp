/*
 * Weapon.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Weapon.h"

#include "../util/Collection.h"
#include "../effects/Bullet.h"
#include "../util/Loader.h"
#include "../util/ResourceManager.h"

Weapon::Weapon(Physical& holder, Collection& collection, b2World& world) :
		Emitter(collection),
		mHolder(holder),
		mBulletTexture(ResourceManager::i().acquire(Loader::i().fromFile<sf::Texture>("bullet.png"))),
		mWorld(world) {
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
	return std::shared_ptr<Particle>(new Bullet(mHolder.getPosition(), mWorld, mBulletTexture,
			mHolder, mHolder.getAngle()));
}
