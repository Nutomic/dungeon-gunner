/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

Enemy::Enemy(b2World& world, const Vector2f& position) :
		Sprite("enemy.png", PhysicalData(position, Vector2i(50, 50), world,
		       CATEGORY_ACTOR, MASK_ALL, true)),
		Actor(100) {

}

Enemy::~Enemy() {
}


void
Enemy::onThink(float elapsedTime) {
}
