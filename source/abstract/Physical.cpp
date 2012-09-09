/*
 * Physical.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#include "Physical.h"

#include <math.h>

#include <Thor/Vectors.hpp>

/**
 * Initializes Box2D body.
 *
 * @param data Data needed for construction.
 */
Physical::Physical(const PhysicalData& data) :
		mDelete(false) {
	assert(data.size != Vector2i());
	assert(data.category);

	b2BodyDef bodyDef;
	// not moving -> static body
	// moving -> dynamic body
	// bullet -> kinematic body
	bodyDef.type = 	(data.moving) ?
						(data.bullet)
							? b2_dynamicBody
							: b2_dynamicBody
						: b2_staticBody;
	bodyDef.position = vector(data.position);
	bodyDef.allowSleep = true;
	bodyDef.fixedRotation = true;
	bodyDef.bullet = data.bullet;
	bodyDef.userData = this;

	mBody = data.world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(pixelToMeter(data.size.x) / 2, pixelToMeter(data.size.y) / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.filter.categoryBits = data.category;
	fixtureDef.filter.maskBits = ~data.maskExclude;
	fixtureDef.restitution = 0;

	mBody->CreateFixture(&fixtureDef);
}

/**
 * Removes body from world.
 */
Physical::~Physical() {
	mBody->GetWorld()->DestroyBody(mBody);
}

/**
 * Initializes container.
 *
 * @link Physical::PhysicalData
 */
Physical::PhysicalData::PhysicalData( const Vector2f& position, const Vector2i& size,
		b2World& world, uint16 category, uint16 maskExclude, bool moving, bool bullet) :
	position(position),
	size(size),
	world(world),
	category(category),
	maskExclude(maskExclude),
	moving(moving),
	bullet(bullet) {
}

/**
 * Returns the position of the sprite (center).
 */
Vector2f
Physical::getPosition() const {
	return vector(mBody->GetPosition());
}

/**
 * Returns the movement speed of the body.
 */
Vector2f
Physical::getSpeed() const {
	return vector(mBody->GetLinearVelocity());
}

/**
 * Returns the rotation of the body (converted to an SFML angle).
 */
float
Physical::getAngle() const {
	return - thor::toDegree(mBody->GetAngle());
}

/**
 * Returns true if this object should be deleted.
 */
bool
Physical::getDelete() const {
	return mDelete;
}

uint16
Physical::getCategory() const {
	return mBody->GetFixtureList()->GetFilterData().categoryBits;
}

/**
 * This method filters collisions with other physicals. Implement it if you want to
 * limit collisions to/with certain objects. Default implementation always returns true.
 *
 * @param other The Physical this object is about to collide with.
 * @return True if the objects should collide.
 */
bool
Physical::doesCollide(Physical& other) {
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
Physical::onCollide(Physical& other, uint16 type) {
}

/**
 * Set to true to mark this object for deletion from the world.
 */
void
Physical::setDelete(bool value) {
	mDelete = value;
}

/**
 * Sets movement speed and direction of the body. Set either value to zero to stop movement.
 *
 * @param direction The direction the body moves in, does not have to be normalized.
 * @param speed The value of the movement speed to be used.
 */
void
Physical::setSpeed(Vector2f direction, float speed) {
	if (direction != Vector2f()) {
		direction = thor::unitVector<float>(direction);
	}
	direction *= speed;
	mBody->SetLinearVelocity(vector(direction));
}

/**
 * Sets the angle of the body based on the direction of a vector.
 */
void
Physical::setAngle(float angle) {
	mBody->SetTransform(mBody->GetPosition(), - thor::toRadian(angle));
}
