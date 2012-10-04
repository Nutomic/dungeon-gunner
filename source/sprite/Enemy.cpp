/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include "Body.h"

Enemy::Enemy(const Instances& instances, const Vector2f& position) :
		Character(instances, "enemy.png", PhysicalData(position, Vector2i(50, 50), instances.world,
		       CATEGORY_ACTOR, MASK_ALL, true, false, true), 100),
		mWorld(instances.world),
		mCollection(instances.collection) {
}

void
Enemy::onThink(float elapsedTime) {
}
