/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include "Body.h"

Enemy::Enemy(const Instances& instances, const Vector2f& position) :
		Character("enemy.png", PhysicalData(position, Vector2i(50, 50), instances.world,
		       CATEGORY_ACTOR, MASK_ALL, true, false, true), 100),
		mWorld(instances.world),
		mCollection(instances.collection) {
}

Enemy::~Enemy() {
	// Insert here to avoid altering b2d data during timestep.
	mCollection.insert(std::shared_ptr<Sprite>(new Body(mWorld, getPosition())));
}

void
Enemy::onThink(float elapsedTime) {
}

void
Enemy::onDeath() {
	setDelete(true);
}
