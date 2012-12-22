/*
 * Body.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#include "Body.h"

#include <math.h>

#include <Thor/Vectors.hpp>

const std::string Body::KEY_SIZE = "size";
const sf::Vector2i Body::DEFAULT_SIZE = sf::Vector2i(50, 50);

/**
 * Initializes Box2D body.
 *
 * @param data Data needed for construction.
 */
Body::Body(const Data& data, const Yaml& config, const sf::Vector2i& pSize) :
		mPosition(data.position),
		mSize(config.get(KEY_SIZE, DEFAULT_SIZE)),
		mAngle(0),
		mCategory(data.category),
		mMask(data.mask),
		mDelete(false) {
}

/**
 * Used to make this class pure virtual without any pure virtual function.
 */
Body::~Body() {
}

/**
 * Initializes container.
 */
Body::Data::Data(const sf::Vector2f& position, float angle,
		Category category, unsigned short maskExclude) :
		position(position),
		angle(angle),
		category(category),
		mask(maskExclude) {
}

/**
 * Returns the position of the sprite (center).
 */
sf::Vector2f
Body::getPosition() const {
	return mPosition;
}

/**
 * Returns the movement speed of the body.
 */
sf::Vector2f
Body::getSpeed() const {
	return mSpeed;
}

/**
 * Returns the rotation of the body (converted to an SFML angle).
 */
float
Body::getAngle() const {
	return mAngle;
}

/**
 * Returns true if this object should be deleted.
 */
bool
Body::getDelete() const {
	return mDelete;
}

/**
 * Returns the Physical::Category of this object.
 */
Body::Category
Body::getCategory() const {
	return mCategory;
}

/**
 * Returns the size of the body as a vector.
 */
sf::Vector2i
Body::getSize() const {
	return mSize;
}

/**
 * This method filters collisions with other physicals. Implement it if you want to
 * limit collisions to/with certain objects. Default implementation always returns true.
 *
 * @param other The Physical this object is about to collide with.
 * @return True if the objects should collide.
 */
bool
Body::doesCollide(Body& other) {
	return true;
}

/**
 * Called when a collision with another body occured. Override this method
 * to manage collision events.
 *
 * @param other Reference to the other Physical in the collision.
 */
void
Body::onCollide(Body& other) {
}

/**
 * Set to true to mark this object for deletion from the world.
 */
void
Body::setDelete(bool value) {
	mDelete = value;
}

/**
 * Sets movement speed and direction of the body. Set either value to zero to stop movement.
 *
 * @param direction The direction the body moves in, does not have to be normalized.
 * @param speed Movement speed in pixels per second.
 */
void
Body::setSpeed(sf::Vector2f direction, float speed) {
	if (direction != sf::Vector2f()) {
		thor::setLength(direction, speed);
	}
	mSpeed = direction;
}

/**
 * Sets the angle of the body.
 */
void
Body::setAngle(float angle) {
	mAngle = angle;
}

/**
 * Sets the position of thr body.
 */
void
Body::setPosition(const sf::Vector2f& position) {
	mPosition = position;
}
