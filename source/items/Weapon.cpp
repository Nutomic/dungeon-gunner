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

const std::string Weapon::KEY_BULLET = "bullet";
const std::string Weapon::DEFAULT_BULLET = "bullet.yaml";
const std::string Weapon::KEY_INTERVAL = "interval";
const int Weapon::DEFAULT_INTERVAL = 250;

Weapon::Weapon(World& world, Body& holder, const Yaml& config) :
		Emitter(world),
		mWorld(world),
		mHolder(holder),
		mBullet(config.get(KEY_BULLET, DEFAULT_BULLET)),
		mTimer(sf::milliseconds(config.get(KEY_INTERVAL, DEFAULT_INTERVAL))) {
	sf::Vector2i holderSize = mHolder.getSize();
	Yaml bullet(mBullet);
	sf::Vector2i bulletSize = bullet.get(Body::KEY_SIZE, sf::Vector2i());
	mOffset = sf::Vector2f(0,
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
	sf::Vector2f offset(- mOffset);
	thor::rotate(offset, mHolder.getAngle());
	return std::shared_ptr<Particle>(new Bullet(mHolder.getPosition() + offset,
			mHolder, mHolder.getAngle(), Yaml(mBullet)));
}
