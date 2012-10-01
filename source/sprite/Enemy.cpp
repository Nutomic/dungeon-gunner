/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include "Body.h"

Enemy::Enemy(b2World& world, const Vector2f& position, Collection& collection) :
		Character("enemy.png", PhysicalData(position, Vector2i(50, 50), world,
		       CATEGORY_ACTOR, MASK_ALL, true, false, true), 100),
		mWorld(world),
		mCollection(collection) {

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
