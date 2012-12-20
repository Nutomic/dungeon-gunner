/*
 * Player.cpp
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#include "Player.h"

#include <Thor/Vectors.hpp>

#include "../items/Weapon.h"
#include "../types/String.h"
#include "../types/Vector.h"

/**
 * Initializes Sprite.
 */
Player::Player(const Instances& instances, const Vector2f& position, const Yaml& config) :
		Character(instances, "player.png", PhysicalData(position, instances.world,
				CATEGORY_ACTOR, MASK_ALL, true, false, true), config),
		mDirection(0) {
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
	Character::fire();
}

/**
 * Moves the player to a destination point.
 * Disables any previous calls to Player::setDirection().
 *
 * @param destination Absolute world coordinate of the destination point.
 */
void
Player::move(const Vector2f& destination) {
	setDestination(destination);
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
		mDirection = mDirection & ~(uint8) direction;
	} else {
		mDirection = mDirection | (uint8) direction;
	}
	// Convert directions into a vector.
	Vector2f dirVec(0, 0);
	if (mDirection & (uint8) Direction::RIGHT) {
		dirVec.x += 1.0f;
	}
	if (mDirection & (uint8) Direction::LEFT) {
		dirVec.x += - 1.0f;
	}
	if (mDirection & (uint8) Direction::DOWN) {
		dirVec.y += 1.0f;
	}
	if (mDirection & (uint8) Direction::UP) {
		dirVec.y += - 1.0f;
	}
	setSpeed(dirVec, getMovementSpeed());
}

/**
 * Check if we arrived at destination, turn towards cursor.
 */
void
Player::onThink(float elapsedTime) {
	if (!mDirection) {
		// Only use path finding movement if no direct input movement active.
		Character::move();
	}
	// Look towards crosshair.
	setAngle(angle(mCrosshairPosition));
}

/**
 * Stop movement if we collide with anything except bullets.
 */
void
Player::onCollide(Body& other, uint16 category) {
	if (category != CATEGORY_PARTICLE) {
		setDestination(getPosition());
	}
}
