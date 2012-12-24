/*
 * Player.cpp
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#include "Player.h"

#include <string>

#include <Thor/Vectors.hpp>

#include "../items/Weapon.h"

/**
 * Initializes Sprite.
 */
Player::Player(World& world, Pathfinder& pathfinder,
	const sf::Vector2f& position, const Yaml& config) :
		Character(world, pathfinder,
				Data(position, 0, CATEGORY_ACTOR, MASK_ALL),
				config),
		mDirection(0) {
}

/**
 * Sets the point where to look and shoot at.
 *
 * @param Absolute world coordinates of the crosshair.
 */
void
Player::setCrosshairPosition(const sf::Vector2f& position) {
	mCrosshairPosition = position - getPosition();
}

/**
 * Pull the trigger on the attached weapon.
 */
void
Player::pullTrigger() {
	Character::pullTrigger();
}

/**
 * Release the trigger on the attached weapon.
 */
void
Player::releaseTrigger() {
	Character::releaseTrigger();
}

/**
 * Moves the player to a destination point.
 * Disables any previous calls to Player::setDirection().
 *
 * @param destination Absolute world coordinate of the destination point.
 */
void
Player::move(const sf::Vector2f& destination) {
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
		mDirection = mDirection & ~direction;
	} else {
		mDirection = mDirection | direction;
	}
	// Convert directions into a vector.
	sf::Vector2f dirVec(0, 0);
	if (mDirection & Direction::RIGHT) {
		dirVec.x += 1.0f;
	}
	if (mDirection & Direction::LEFT) {
		dirVec.x += - 1.0f;
	}
	if (mDirection & Direction::DOWN) {
		dirVec.y += 1.0f;
	}
	if (mDirection & Direction::UP) {
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
	if (mCrosshairPosition != sf::Vector2f()) {
		setAngle(thor::polarAngle(mCrosshairPosition) + 90);
	}
}

/**
 * Stop movement if we collide with anything except bullets.
 */
void
Player::onCollide(Sprite& other) {
	if (other.getCategory() != CATEGORY_PARTICLE) {
		setDestination(getPosition());
	}
}
