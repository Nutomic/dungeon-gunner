/*
 * Player.cpp
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#include "Player.h"

#include <Thor/Vectors.hpp>

#include "../util/Yaml.h"

const std::string Player::CONFIG = "player.yaml";

/**
 * Initializes Sprite.
 */
Player::Player(World& world, Pathfinder& pathfinder,
		const sf::Vector2f& position) :
	Character(position, CATEGORY_ACTOR, MASK_ALL, Yaml(CONFIG), world,
			pathfinder),
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
 * Sets the movement direction. This is destined for input via keys (eg. WASD).
 * Disables any previous commands via Player::move().
 *
 * @param direction The direction to move to.
 * @param unset False to start movement into the direction, true to stop it.
 */
void
Player::setDirection(Direction direction, bool unset) {
	if (unset)
		mDirection = mDirection & ~direction;
	else
		mDirection = mDirection | direction;
		
	// Convert directions into a vector.
	sf::Vector2f dirVec(0, 0);
	if (mDirection & Direction::RIGHT)
		dirVec.x += 1.0f;
	if (mDirection & Direction::LEFT)
		dirVec.x += - 1.0f;
	if (mDirection & Direction::DOWN)
		dirVec.y += 1.0f;
	if (mDirection & Direction::UP)
		dirVec.y += - 1.0f;
	setSpeed(dirVec, getMovementSpeed());
}

void
Player::setDestination(const sf::Vector2f& destination) {
	mDirection = 0;
	Character::setDestination(destination);
}

/**
 * Check if we arrived at destination, turn towards cursor.
 */
void
Player::onThink(int elapsed) {
	Character::onThink(elapsed);
	Sprite::setDirection(mCrosshairPosition);
}
