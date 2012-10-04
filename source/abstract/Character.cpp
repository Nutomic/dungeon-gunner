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

std::vector<Character*> Character::mCharacterInstances = std::vector<Character*>();

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(const Instances& instances, const sf::String& texturePath,
	const PhysicalData& data, int health) :
		Sprite(texturePath, data),
		mMaxHealth(health),
		mCurrentHealth(health),
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
			getPosition())));
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
 * Subtracts health from Actor.
 *
 * @param damage Amount of health to subtract.
 */
void
Character::onDamage(int damage) {
	mCurrentHealth -= damage;
	if (mCurrentHealth <= 0) {
		mCurrentHealth = 0;
		onDeath();
	}
}

/**
 * Called when health reaches zero. Marks the object for deletion.
 *
 * @warning Implementations should call the default implementation.
 */
void
Character::onDeath() {
	setDelete(true);
}
