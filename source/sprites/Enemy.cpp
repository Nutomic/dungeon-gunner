/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include <Thor/Vectors.hpp>

Enemy::Enemy(World& world, TileManager& tileManager,
		const sf::Vector2f& position, const Yaml& config) :
		Character(world, tileManager, Data(position, CATEGORY_ACTOR, MASK_ALL),
				config) {
}

void
Enemy::onThink(int elapsed) {
	Character::onThink(elapsed);

	auto characters = getCharacters();
	std::shared_ptr<Character> target;
	float distanceSquared = std::numeric_limits<float>::max();
	for (auto it : characters) {
		if (distanceSquared > thor::squaredLength(it->getPosition() - getPosition())) {
			target = it;
			distanceSquared = thor::squaredLength(it->getPosition() - getPosition());
		}
	}
	if (target) {
		if (isVisible(target->getPosition())) {
			setDestination(getPosition());
			setDirection(target->getPosition() - getPosition());
			pullTrigger();
		}
		else if (isMoving())
			move();
		else
			setDestination(target->getPosition());
	}
	else
		releaseTrigger();
}
