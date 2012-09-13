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
const Vector2i Player::SIZE = Vector2i(50, 50);

/**
 * Initializes Sprite.
 */
Player::Player(b2World& world, Collection& collection, const Vector2f& position) :
		Sprite("player.png", PhysicalData(position, SIZE, world,
				CATEGORY_ACTOR, MASK_ALL, true, false, true)),
		Actor(100),
		mWeapon(*this, collection, world, SIZE),
		mDestination(Vector2i(position)),
		mDirection(0),
		mDirectInput(false) {
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
 * Fires the attached Weapon, emitting a Bullet object.
 */
void
Player::fire() {
	mWeapon.fire();
}

/**
 * Moves the player to a destination point.
 * Disables any previous calls to Player::setDirection().
 *
 * @param destination Absolute world coordinate of the destination point.
 */
void
Player::move(const Vector2f& destination) {
	mDestination = destination;
	// Convert to relative destination.
	setSpeed(mDestination - getPosition(), SPEED);
	mDirectInput = false;
}

/**
 * Sets the movement direction. This is destined for input via keys (eg. WASD).
 * Disables any previous commands via Player::move().
 *
 * @param direction The direction to move to.
 * @param unset False to start movement into the direction, true to stop it.
 */
void
Player::setDirection(Direction direction, bool unset) {
	if (unset) {
		mDirection = mDirection & ~direction;
	} else {
		mDirection = mDirection | direction;
	}
	// Convert directions into a vector.
	Vector2f dirVec(0, 0);
	if (mDirection & DIRECTION_RIGHT) {
		dirVec.x += 1.0f;
	}
	if (mDirection & DIRECTION_LEFT) {
		dirVec.x += - 1.0f;
	}
	if (mDirection & DIRECTION_DOWN) {
		dirVec.y += 1.0f;
	}
	if (mDirection & DIRECTION_UP) {
		dirVec.y += - 1.0f;
	}
	setSpeed(dirVec, SPEED);
	mDirectInput = true;
}

/**
 * Check if we arrived at destination, turn towards cursor.
 */
void
Player::onThink(float elapsedTime) {
	// Stop if we are close enough to destination.
	if (!mDirectInput && (thor::length(mDestination - getPosition()) < 1.0f)) {
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
