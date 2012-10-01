/*
 * Bullet.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Bullet.h"

#include "../abstract/Character.h"

const Vector2i Bullet::SIZE = Vector2i(20, 20);

const float Bullet::SPEED = 500.0f;

/**
 * Places a bullet in the world.
 *
 * @param position World position of the bullet.
 * @param world Box2d world.
 * @param texture Texture to display for bullet.
 */
Bullet::Bullet(const Vector2f& position, b2World& world,
	const std::shared_ptr<sf::Texture>& texture, Physical& shooter, float direction, int damage) :
		Particle(texture, PhysicalData(position, SIZE, world, CATEGORY_PARTICLE,
				CATEGORY_PARTICLE, true, true, true)),
		mShooter(shooter),
		mDamage(damage) {
	setSpeed(angle(direction), SPEED);
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
