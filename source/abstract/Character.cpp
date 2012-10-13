/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Character.h"

#include <algorithm>
#include <assert.h>

#include "../sprite/Body.h"

const String Character::KEY_HEALTH = "health";
const String Character::KEY_SPEED = "speed";
std::vector<Character*> Character::mCharacterInstances = std::vector<Character*>();

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(const Instances& instances, const String& texturePath,
	const PhysicalData& data, const Yaml& config) :
		Sprite(config, data),
		mMaxHealth(config.get<int>(KEY_HEALTH)),
		mCurrentHealth(mMaxHealth),
		mMovementSpeed(config.get<float>(KEY_SPEED)),
		mInstances(instances) {
		mCharacterInstances.push_back(this);
}

/**
 * Deletes pointer from static variable mCharacterInstances, inserts body into world
 * (done here to avoid altering Box2D data during timestep).
 */
Character::~Character() {
	auto it = std::find(mCharacterInstances.begin(), mCharacterInstances.end(), this);
	assert(it != mCharacterInstances.end());
	mCharacterInstances.erase(it);

	mInstances.collection.insert(std::shared_ptr<Sprite>(new Body(mInstances.world,
			getPosition(), Yaml("body.yaml"))));
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
