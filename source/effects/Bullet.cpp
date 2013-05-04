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


/**
 * Places a bullet in the world.
 *
 * @param position World position of the bullet.
 * @param world Box2d world.
 * @param texture Texture to display for bullet.
 */
Bullet::Bullet(const sf::Vector2f& position, Sprite& shooter,
		sf::Vector2f direction, const Yaml& config) :
		Particle(config, Data(position, CATEGORY_PARTICLE, ~CATEGORY_PARTICLE,
				thor::rotatedVector(direction, -90.0f))),
		mShooter(shooter),
		mDamage(config.get(YAML_KEY::DAMAGE, YAML_DEFAULT::DAMAGE)),
		mSpeed(config.get(YAML_KEY::SPEED, YAML_DEFAULT::SPEED)) {
	setSpeed(direction, mSpeed);
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
