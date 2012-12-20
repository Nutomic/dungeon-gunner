/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include "Corpse.h"

Enemy::Enemy(const Instances& instances, const Vector2f& position, const Yaml& config) :
		Character(instances, "enemy.png", PhysicalData(position, instances.world,
		       CATEGORY_ACTOR, MASK_ALL, true, false, true), config),
		mWorld(instances.world),
		mCollection(instances.collection) {
}

void
Enemy::onThink(float elapsedTime) {
}
