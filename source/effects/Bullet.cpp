/*
 * Bullet.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Bullet.h"

#include "../abstract/Character.h"
#include "../util/Loader.h"
#include "../util/ResourceManager.h"

const Vector2i Bullet::SIZE = Vector2i(20, 20);
const String Bullet::KEY_DAMAGE = "damage";
const String Bullet::KEY_SPEED = "speed";

/**
 * Places a bullet in the world.
 *
 * @param position World position of the bullet.
 * @param world Box2d world.
 * @param texture Texture to display for bullet.
 */
Bullet::Bullet(const Vector2f& position, b2World& world, Physical& shooter, float direction,
	const Yaml& config) :
		Particle(ResourceManager::i().acquire(Loader::i().fromFile<sf::Texture>("bullet.png")),
				PhysicalData(position, world, CATEGORY_PARTICLE, CATEGORY_PARTICLE,
				true, true, true), config),
		mShooter(shooter),
		mDamage(config.get<int>(KEY_DAMAGE)),
		mSpeed(config.get<int>(KEY_SPEED)) {
	setSpeed(angle(direction), mSpeed);
	setAngle(direction);
}

/**
 * @copydoc Physical::onCollide
 */
void
Bullet::onCollide(Physical& other, uint16 type) {
	// Make sure we do not damage twice.
	if (!getDelete()) {
		// Call onShot on other, with damage as param.
		if (type == CATEGORY_ACTOR) {
			Character& a = dynamic_cast<Character&>(other);
			a.onDamage(mDamage);
		}
		setDelete(true);
	}
}

bool
Bullet::doesCollide(Physical& other) {
	return &other != &mShooter;
}
