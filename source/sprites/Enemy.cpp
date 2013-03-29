/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

Enemy::Enemy(World& collection, const sf::Vector2f& position, const Yaml& config) :
		Character(collection, Data(position, 0, CATEGORY_ACTOR, MASK_ALL),
				config) {
}
