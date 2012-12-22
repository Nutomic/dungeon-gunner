/*
 * Body.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#include "Body.h"

#include <math.h>

#include <Thor/Vectors.hpp>

const String Body::KEY_SIZE = "size";
const Vector2i Body::DEFAULT_SIZE = Vector2i(50, 50);

/**
 * Initializes Box2D body.
 *
 * @param data Data needed for construction.
 */
Body::Body(const PhysicalData& data, const Yaml& config, const Vector2i& pSize) :
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
Body::PhysicalData::PhysicalData( const Vector2f& position, World& world, Category category,
	unsigned short maskExclude, bool moving, bool bullet, bool circle) :
		position(position),
		world(world),
		category(category),
		maskExclude(maskExclude),
		moving(moving),
		bullet(bullet),
		circle(circle) {
}

/**
 * Returns the position of the sprite (center).
 */
Vector2f
Body::getPosition() const {
	return Vector2f();
}

/**
 * Returns the movement speed of the body.
 */
Vector2f
Body::getSpeed() const {
	return Vector2f();
}

/**
 * Returns the rotation of the body (converted to an SFML angle).
 */
float
Body::getAngle() const {
	return 0;
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
	return CATEGORY_WORLD;
}

/**
 * Returns the size of the body as a vector.
 */
Vector2f
Body::getSize() const {
	return Vector2f();
}

/**
 * Returns true if collisions are enabled for the body.
 */
bool
Body::isSolid() const {
	return false;
}

/**
 * Returns true if the body is able to move.
 */
bool
Body::isMovable() const {
	return false;
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
 * @param category The Category of the other object (as passed in constructor).
 */
void
Body::onCollide(Body& other, Category type) {
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
 * @param speed The value of the movement speed to be used.
 */
void
Body::setSpeed(Vector2f direction, float speed) {
}

/**
 * Sets the angle of the body based on the direction of a vector.
 */
void
Body::setAngle(float angle) {
}
