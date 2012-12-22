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

const String Character::KEY_HEALTH = "health";
const int Character::DEFAULT_HEALTH = 100;
const String Character::KEY_SPEED = "speed";
const float Character::DEFAULT_SPEED = 100;
const float Character::POINT_REACHED_DISTANCE = 1.0f;
std::vector<Character*> Character::mCharacterInstances = std::vector<Character*>();

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(World& world, Pathfinder& pathfinder,
		const Data& data, const Yaml& config) :
		Sprite(config, data),
		mWorld(world),
		mPathfinder(pathfinder),
		mMaxHealth(config.get(KEY_HEALTH, DEFAULT_HEALTH)),
		mCurrentHealth(mMaxHealth),
		mMovementSpeed(config.get(KEY_SPEED, DEFAULT_SPEED)),
		mWeapon(world, *this, Yaml("weapon.yaml")),
		mStartPathfinding(false) {
		mCharacterInstances.push_back(this);
}

/**
 * Deletes pointer from static variable mCharacterInstances, inserts body into world.
 */
Character::~Character() {
	auto it = std::find(mCharacterInstances.begin(), mCharacterInstances.end(), this);
	assert(it != mCharacterInstances.end());
	mCharacterInstances.erase(it);

	mWorld.insert(std::shared_ptr<Sprite>(new Corpse(getPosition(), Yaml("body.yaml"))));
}

/**
 * Calls onThink on all Actor instances.
 *
 * @param elapsedTime Amount of time to simulate.
 */
void
Character::think(float elapsedTime) {
	for (auto i : mCharacterInstances) {
		i->onThink(elapsedTime);
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
 * Implement this function for any (regular) AI computations. Default implementation does nothing.
 *
 * @param elapsedTime Amount of time to simulate.
 */
void
Character::onThink(float elapsedTime) {
}

/**
 * Called when health reaches zero. Default immplementation does nothing.
 */
void
Character::onDeath() {
}

/**
 * Gets the default movement speed (walking) of the character.
 */
float
Character::getMovementSpeed() const {
	return mMovementSpeed;
}

/**
 * Fire the attached weapon.
 */
void
Character::fire() {
	mWeapon.fire();
}

/**
 * Set a destination to be walked to. Call move() for the actual movement.
 *
 * @param destination An absolute point to move towards.
 * @return True if a path was found.
 */
bool
Character::setDestination(const Vector2f& destination) {
	mPath = mPathfinder.getPath(*this, destination);
	// Make sure we found a path.
	if (mPath.empty()) {
		LOG_I("No path found to destination.");
	} else {
		mStartPathfinding = true;
	}
	return mStartPathfinding;
}

/**
 * Move towards a destination. Call setDestination() for setting the destination.
 * This should be called from think().
 */
void
Character::move() {
	if (!mPath.empty()) {
		if (mStartPathfinding) {
			setSpeed(*mPath.end() - getPosition(), mMovementSpeed);
		}
		if (thor::length(*mPath.end() - getPosition()) < POINT_REACHED_DISTANCE) {
			// Reached a path node.
			mPath.pop_back();
			if (!mPath.empty()) {
				// Move to next path node.
				setSpeed(*mPath.end() - getPosition(), mMovementSpeed);
			}
			else {
				LOG_I("Reached destination.");
				setSpeed(Vector2f(), 0);
			}
		}
	}
}
