/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Character.h"

#include <Thor/Vectors.hpp>

#include "../items/Weapon.h"
#include "../sprites/Corpse.h"
#include "../sprites/TileManager.h"
#include "../util/Log.h"
#include "../util/Yaml.h"
#include "../World.h"

const std::string Character::KEY_HEALTH = "health";
const int Character::DEFAULT_HEALTH = 100;
const std::string Character::KEY_SPEED = "speed";
const float Character::DEFAULT_SPEED = 100;
const float Character::POINT_REACHED_DISTANCE = 1.0f;
const std::string Character::KEY_WEAPON = "weapon";
const std::string Character::DEFAULT_WEAPON = "weapon.yaml";
const float Character::VISION_DISTANCE = 500.0f;

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(World& world, TileManager& tileManager, const Data& data,
	const Yaml& config) :
		Sprite(data, config),
		mWorld(world),
		mTileManager(tileManager),
		mMaxHealth(config.get(KEY_HEALTH, DEFAULT_HEALTH)),
		mCurrentHealth(mMaxHealth),
		mMovementSpeed(config.get(KEY_SPEED, DEFAULT_SPEED)),
		mWeapon(new Weapon(world, *this,
				Yaml(config.get(KEY_WEAPON, DEFAULT_WEAPON)))) {
}

Character::~Character() {
}

/**
 * Subtracts health from Actor. Calls onDeath() when health reaches zero and marks
 * object for deletion.
 *
 * @param damage Amount of health to subtract.
 */
void
Character::onDamage(int damage) {
	mCurrentHealth -= damage;
	if (mCurrentHealth <= 0) {
		mCurrentHealth = 0;
		onDeath();
		setDelete(true);
	}
}

/**
 * Implement this function for any (periodical) AI computations.
 * If overwritten, this function should always be called from the overwriting function.
 *
 * @param elapsed Amount of time to simulate.
 */
void
Character::onThink(int elapsed) {
	mWeapon->onThink(elapsed);
	move();
}

/**
 * Called when health reaches zero. Default implementation drops a corpse at
 * the current position.
 */
void
Character::onDeath() {
	mWorld.insert(std::shared_ptr<Sprite>(new Corpse(getPosition(), Yaml("body.yaml"))));
}

/**
 * Gets the default movement speed (walking) of the character.
 */
float
Character::getMovementSpeed() const {
	return mMovementSpeed;
}

/**
 * Pull the trigger on the attached weapon.
 */
void
Character::pullTrigger() {
	mWeapon->pullTrigger();
}

/**
 * Release the trigger on the attached weapon.
 */
void
Character::releaseTrigger() {
	mWeapon->releaseTrigger();
}

/**
 * Set a destination to be walked to. Call move() to actually
 * perform the movement.
 *
 * @param destination An absolute point to move towards. Set to current
 * 						position to stop movement.
 * @return True if a path was found.
 */
bool
Character::setDestination(const sf::Vector2f& destination) {
	if (destination == getPosition()) {
		mPath.clear();
		return true;
	}
	mPath = mWorld.getPath(getPosition(), destination, getRadius());
	if (!mPath.empty())
		setSpeed(mPath.back() - getPosition(), mMovementSpeed);
	else
		LOG_W("No path found to destination.");
	return !mPath.empty();
}

/**
 * Move towards a destination. Call setDestination() for setting the destination.
 * This is automatically called from onThink().
 */
void
Character::move() {
	if (!mPath.empty()) {
		if (thor::length(mPath.back() - getPosition()) < POINT_REACHED_DISTANCE) {
			mPath.pop_back();
			(!mPath.empty())
				? setSpeed(mPath.back() - getPosition(), mMovementSpeed)
				: setSpeed(sf::Vector2f(), 0);
		}
	}
}

/**
 * Returns true if the character is currently moving.
 */
bool
Character::isMoving() const {
	return !mPath.empty();
}

/**
 * Tests if a target is visible from the current position.
 */
bool
Character::isVisible(const sf::Vector2f& target) const {
	return mTileManager.raycast(getPosition(), target);
}

/**
 * Calls World::getCharacters with current position.
 */
std::vector<std::shared_ptr<Character> >
Character::getCharacters() const {
	return mWorld.getCharacters(getPosition(), VISION_DISTANCE);
}
