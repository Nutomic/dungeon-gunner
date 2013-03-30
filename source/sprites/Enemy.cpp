/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

Enemy::Enemy(World& world, const sf::Vector2f& position,
		const Yaml& config) :
		Character(world, Data(position, sf::Vector2f(0, 0),
				CATEGORY_ACTOR, MASK_ALL), config) {
}
