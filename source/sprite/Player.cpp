/*
 * Player.cpp
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#include "Player.h"

#include <Thor/Vectors.hpp>

#include "../util/Vector.h"
#include "../items/Weapon.h"

const float Player::SPEED = 100.0f;

/**
 * Initializes Sprite.
 */
Player::Player(b2World& world, Collection& collection) :
		Sprite("player.png", PhysicalData(Vector2f(200.0f, 100.0f), Vector2i(50, 50), world,
				CATEGORY_ACTOR, MASK_ALL, true)),
		mWeapon(*this, collection, world),
		mDestination(Vector2i(50, 50)) {
}

/**
 * Sets the point where to look and shoot at.
 *
 * @param Absolute world coordinates of the crosshair.
 */
void
Player::setCrosshairPosition(const Vector2f& position) {
	mCrosshairPosition = position - getPosition();
}
/**
 * Fire the attacked Weapon, emitting a Bullet object.
 */
void
Player::fire() {
	mWeapon.fire();
}

/**
 * Moves the player to a destination point.
 *
 * @param destination Absolute world coordinate of the destination point.
 */
void
Player::move(const Vector2f& destination) {
	mDestination = destination;
	// Convert to relative destination.
	setSpeed(mDestination - getPosition(), SPEED);
}

void
Player::onThink(float elapsedTime) {
	// Stop if we are close enough.
	if (thor::length(mDestination - getPosition()) < 1.0f) {
		setSpeed(Vector2f(), 0);
	}
	// Look towards crosshair.
	setAngle(angle(mCrosshairPosition));
}

/**
 * Stop movement if we collide with anything except bullets.
 */
void
Player::onCollide(Physical& other, uint16 category) {
	if (category != CATEGORY_PARTICLE) {
		mDestination = getPosition();
	}
}
