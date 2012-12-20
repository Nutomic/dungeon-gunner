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

/**
 * Initializes Box2D body.
 *
 * @param data Data needed for construction.
 */
Body::Body(const PhysicalData& data, const Yaml& config, const Vector2i& pSize) :
		mDelete(false) {
	Vector2i size = (pSize == Vector2i())
		? config.get<Vector2i>(KEY_SIZE)
		: pSize;
	assert(size != Vector2i());

	b2BodyDef bodyDef;
	bodyDef.type = (data.moving)
						? b2_dynamicBody
						: b2_staticBody;
	bodyDef.position = vector(data.position);
	bodyDef.allowSleep = true;
	bodyDef.fixedRotation = true;
	bodyDef.bullet = data.bullet;
	bodyDef.userData = this;

	mBody = data.world.CreateBody(&bodyDef);

	b2Shape* shape;
	if (data.circle) {
		assert(size.x == size.y);
		shape = new b2CircleShape;
		shape->m_radius = pixelToMeter(size.x) / 2;
	}
	else {
		b2PolygonShape* box = new b2PolygonShape;
		box->SetAsBox(pixelToMeter(size.x) / 2,
				      pixelToMeter(size.y) / 2);
		shape = dynamic_cast<b2Shape*>(box);
	}

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1.0f;
	fixtureDef.filter.categoryBits = data.category;
	fixtureDef.filter.maskBits = ~data.maskExclude;
	fixtureDef.restitution = 0;
	fixtureDef.density = (data.bullet) ? 0 : 10000;

	mBody->CreateFixture(&fixtureDef);

	delete shape;
}

/**
 * Removes body from world.
 */
Body::~Body() {
	mBody->GetWorld()->DestroyBody(mBody);
}

/**
 * Initializes container.
 *
 * @link Physical::PhysicalData
 */
Body::PhysicalData::PhysicalData( const Vector2f& position, b2World& world, uint16 category,
	uint16 maskExclude, bool moving, bool bullet, bool circle) :
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
	return vector(mBody->GetPosition());
}

/**
 * Returns the movement speed of the body.
 */
Vector2f
Body::getSpeed() const {
	return vector(mBody->GetLinearVelocity());
}

/**
 * Returns the rotation of the body (converted to an SFML angle).
 */
float
Body::getAngle() const {
	return thor::toDegree(mBody->GetAngle());
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
	return (Category) mBody->GetFixtureList()->GetFilterData().categoryBits;
}

/**
 * Returns the size of the body as a vector.
 */
Vector2f
Body::getSize() const {
	b2AABB aabb(mBody->GetFixtureList()->GetAABB(0));
	return vector(aabb.upperBound - aabb.lowerBound);
}

/**
 * Returns true if collisions are enabled for the body.
 */
bool
Body::isSolid() const {
	return mBody->GetFixtureList()->GetFilterData().maskBits != 0;
}

/**
 * Returns true if the body is able to move.
 */
bool
Body::isMovable() const {
	return mBody->GetType() != b2_staticBody;
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
Body::onCollide(Body& other, uint16 type) {
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
Body::setAngle(float angle) {
	mBody->SetTransform(mBody->GetPosition(), thor::toRadian(angle));
}
