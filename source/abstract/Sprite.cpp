/*
 * Sprite.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#include "Sprite.h"

#include <Thor/Vectors.hpp>

#include "../util/Loader.h"
#include "../util/Log.h"
#include "../util/ResourceManager.h"

Sprite::Sprite(const sf::Vector2f& position, Category category,
			unsigned short mask, const sf::Vector2f& size,
			const std::string& texture,	const sf::Vector2f& direction) :
			mCategory(category),
			mMask(mask),
			mDelete(false) {
	mShape.setSize(size);
	mShape.setOrigin(size / 2.0f);
	mShape.setTextureRect(sf::IntRect(sf::Vector2i(), sf::Vector2i(size)));
	setPosition(position);
	setDirection(direction);
	try {
		mTexture = ResourceManager::i().acquire(Loader::i()
				.fromFile<sf::Texture>(texture));
		mShape.setTexture(&*mTexture, false);
	}
	catch (thor::ResourceLoadingException&) {
		LOG_W("Failed to load texture " << texture << ", coloring red.");
		mShape.setFillColor(sf::Color(255, 0, 0));
	}
}

/**
 * Returns the position of the sprite (center).
 */
sf::Vector2f
Sprite::getPosition() const {
	return mShape.getPosition();
}

/**
 * Returns the movement speed of the sprite.
 */
sf::Vector2f
Sprite::getSpeed() const {
	return mSpeed;
}

/**
 * Returns the angle of the sprite.
 */
sf::Vector2f
Sprite::getDirection() const {
	return thor::rotatedVector(sf::Vector2f(1, 0), mShape.getRotation());
}

/**
 * Returns true if this object should be deleted.
 */
bool
Sprite::getDelete() const {
	return mDelete;
}

/**
 * Returns the Category of this object.
 */
Sprite::Category
Sprite::getCategory() const {
	return mCategory;
}

/**
 * Returns the size of the sprite as a vector (bottom left to top right),
 * does not consider rotation.
 */
sf::Vector2f
Sprite::getSize() const {
	sf::FloatRect bounds = mShape.getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void
Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mShape, states);
}

/**
 * Returns true if collisions with that category are enabled through mask.
 */
bool
Sprite::collisionEnabled(Category category) const {
	return (category & mMask) != 0;
}

bool
Sprite::isInside(const sf::FloatRect& rect) const {
	return rect.intersects(mShape.getGlobalBounds());
}
/**
 * Called when a collision with another Sprite occured. Override this method
 * to manage collision events.
 *
 * @param other The other Sprite in the collision.
 */
void
Sprite::onCollide(std::shared_ptr<Sprite> other) {
}

/**
 * Set to true to mark this object for deletion from the world.
 */
void
Sprite::setDelete(bool value) {
	mDelete = value;
}

/**
 * Sets movement speed and direction of the Sprite. Set either value to zero to stop movement.
 *
 * @param direction The direction the Sprite moves in, does not have to be normalized.
 * @param speed Movement speed in pixels per second.
 */
void
Sprite::setSpeed(sf::Vector2f direction, float speed) {
	if (direction != sf::Vector2f()) 
		thor::setLength(direction, speed);
	mSpeed = direction;
}

/**
 * Rotates sprite in the direction of the vector. Vector length must not be null,
 * but is otherwise meaningless.
 */
void
Sprite::setDirection(const sf::Vector2f& direction) {
	if (direction != sf::Vector2f())
		mShape.setRotation(thor::polarAngle(direction) + 90);
}

/**
 * Sets the position of thr Sprite.
 */
void
Sprite::setPosition(const sf::Vector2f& position) {
	mShape.setPosition(position);
}
