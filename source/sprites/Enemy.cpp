/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include "Corpse.h"

Enemy::Enemy(World& world, Collection& collection, Pathfinder& pathfinder,
	const Vector2f& position, const Yaml& config) :
		Character(world, collection, pathfinder, "enemy.png",
				PhysicalData(position, world, CATEGORY_ACTOR, MASK_ALL,
						true, false, true),
				config),
		mWorld(world),
		mCollection(collection) {
}

void
Enemy::onThink(float elapsedTime) {
}
