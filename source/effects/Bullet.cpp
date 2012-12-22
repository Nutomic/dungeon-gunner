/*
 * Bullet.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Bullet.h"

#include <Thor/Vectors.hpp>

#include "../abstract/Character.h"
#include "../util/Loader.h"
#include "../util/ResourceManager.h"

const String Bullet::KEY_DAMAGE = "damage";
const int Bullet::DEFAULT_DAMAGE = 10;
const String Bullet::KEY_SPEED = "speed";
const float Bullet::DEFAULT_SPEED = 500;

/**
 * Places a bullet in the world.
 *
 * @param position World position of the bullet.
 * @param world Box2d world.
 * @param texture Texture to display for bullet.
 */
Bullet::Bullet(const Vector2f& position, World& world, Body& shooter, float direction,
	const Yaml& config) :
		Particle(config, PhysicalData(position, world, CATEGORY_PARTICLE, CATEGORY_PARTICLE,
				true, true, true)),
		mShooter(shooter),
		mDamage(config.get(KEY_DAMAGE, DEFAULT_DAMAGE)),
		mSpeed(config.get(KEY_SPEED, DEFAULT_SPEED)) {
	Vector2f dir(1.0f, 0);
	thor::setPolarAngle(dir, direction);
	setSpeed(dir, mSpeed);
	setAngle(direction);
}

/**
 * @copydoc Physical::onCollide
 */
void
Bullet::onCollide(Body& other, Category type) {
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
Bullet::doesCollide(Body& other) {
	return &other != &mShooter;
}
