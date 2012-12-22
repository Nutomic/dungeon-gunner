/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include "Corpse.h"

Enemy::Enemy(World& collection, Pathfinder& pathfinder,
	const Vector2f& position, const Yaml& config) :
		Character(collection, pathfinder,
				Data(position, 0, CATEGORY_ACTOR, MASK_ALL),
				config) {
}

void
Enemy::onThink(float elapsedTime) {
}
