/*
 * Bullet.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Bullet.h"

#include <Thor/Vectors.hpp>

#include "../abstract/Character.h"
#include "../util/Yaml.h"

const std::string Bullet::KEY_DAMAGE = "damage";
const int Bullet::DEFAULT_DAMAGE = 10;
const std::string Bullet::KEY_SPEED = "speed";
const float Bullet::DEFAULT_SPEED = 500;

/**
 * Places a bullet in the world.
 *
 * @param position World position of the bullet.
 * @param world Box2d world.
 * @param texture Texture to display for bullet.
 */
Bullet::Bullet(const sf::Vector2f& position, Sprite& shooter,
		sf::Vector2f direction, const Yaml& config) :
		Particle(config, Data(position, CATEGORY_PARTICLE,
				~CATEGORY_PARTICLE)),
		mShooter(shooter),
		mDamage(config.get(KEY_DAMAGE, DEFAULT_DAMAGE)),
		mSpeed(config.get(KEY_SPEED, DEFAULT_SPEED)) {
	thor::rotate(direction, - 90.0f);
	setSpeed(direction, mSpeed);
	setDirection(direction);
}

/**
 * @copydoc Physical::onCollide
 */
void
Bullet::onCollide(std::shared_ptr<Sprite> other) {
	// Make sure we do not damage twice.
	if (!getDelete()) {
		// Call onShot on other, with damage as param.
		if (other->getCategory() == CATEGORY_ACTOR) {
			std::shared_ptr<Character> character = std::static_pointer_cast<Character>(other);
			character->onDamage(mDamage);
		}
		setDelete(true);
	}
}
