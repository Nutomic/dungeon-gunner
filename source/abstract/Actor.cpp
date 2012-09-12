/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Actor.h"

#include <algorithm>
#include <assert.h>

std::vector<Actor*> Actor::mInstances = std::vector<Actor*>();

/**
 * Saves pointer to this instance in static var for think().
 */
Actor::Actor(int health) :
	mMaxHealth(health),
	mCurrentHealth(health) {
	mInstances.push_back(this);
}

/**
 * Deletes pointer from think() static var.
 */
Actor::~Actor() {
	auto it = std::find(mInstances.begin(), mInstances.end(), this);
	assert(it != mInstances.end());
	mInstances.erase(it);
}

/**
 * Calls onThink on all Actor instances.
 *
 * @param elapsedTime Amount of time to simulate.
 */
void
Actor::think(float elapsedTime) {
	for (auto i : mInstances) {
		i->onThink(elapsedTime);
	}
}

/**
 * Subtracts health from Actor.
 *
 * @param damage Amount of health to subtract.
 */
void
Actor::onDamage(int damage) {
	mCurrentHealth -= damage;
	if (mCurrentHealth <= 0) {
		mCurrentHealth = 0;
		onDeath();
	}
}

/**
 * Called when health reaches zero. Does nothing by default.
 */
void
Actor::onDeath() {
}
