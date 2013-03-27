/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Character.h"

#include <algorithm>
#include <assert.h>

#include <Thor/Vectors.hpp>

#include "../sprites/Corpse.h"
#include "../util/Log.h"

const std::string Character::KEY_HEALTH = "health";
const int Character::DEFAULT_HEALTH = 100;
const std::string Character::KEY_SPEED = "speed";
const float Character::DEFAULT_SPEED = 100;
const float Character::POINT_REACHED_DISTANCE = 1.0f;
const std::string Character::KEY_WEAPON = "weapon";
const std::string Character::DEFAULT_WEAPON = "weapon.yaml";
std::vector<Character*> Character::mCharacterInstances = std::vector<Character*>();

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(World& world, const Data& data, const Yaml& config) :
		Sprite(data, config),
		mWorld(world),
		mMaxHealth(config.get(KEY_HEALTH, DEFAULT_HEALTH)),
		mCurrentHealth(mMaxHealth),
		mMovementSpeed(config.get(KEY_SPEED, DEFAULT_SPEED)),
		mWeapon(world, *this, Yaml(config.get(KEY_WEAPON, DEFAULT_WEAPON))),
		mStartPathfinding(false) {
		mCharacterInstances.push_back(this);
}

/**
 * Deletes pointer from static variable mCharacterInstances.
 */
Character::~Character() {
	auto it = std::find(mCharacterInstances.begin(), mCharacterInstances.end(), this);
	assert(it != mCharacterInstances.end());
	mCharacterInstances.erase(it);
}

/**
 * Calls onThink on all Actor instances.
 *
 * @param elapsed Amount of time to simulate.
 */
void
Character::think(int elapsed) {
	for (auto i : mCharacterInstances) {
		i->onThink(elapsed);
	}
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
 * Implement this function for any (regular) AI computations.
 * If overwritten, this function should always be called from the overwriting function.
 *
 * @param elapsed Amount of time to simulate.
 */
void
Character::onThink(int elapsed) {
	mWeapon.onThink(elapsed);
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
	mWeapon.pullTrigger();
}

/**
 * Release the trigger on the attached weapon.
 */
void
Character::releaseTrigger() {
	mWeapon.releaseTrigger();
}

/**
 * Set a destination to be walked to. Call move() for the actual movement.
 *
 * @param destination An absolute point to move towards.
 * @return True if a path was found.
 */
bool
Character::setDestination(const sf::Vector2f& destination) {
	mPath = mWorld.getPath(getPosition(), destination, 2 * getRadius());
	if (!mPath.empty()) {
		setSpeed(mPath.back() - getPosition(), mMovementSpeed);
	}
	else {
		LOG_W("No path found to destination.");
	}
	return !mPath.empty();
}

/**
 * Move towards a destination. Call setDestination() for setting the destination.
 * This should be called from think().
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
