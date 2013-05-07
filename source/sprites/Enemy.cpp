/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include <Thor/Vectors.hpp>

#include "../util/Yaml.h"

const std::string Enemy::CONFIG = "enemy.yaml";

Enemy::Enemy(World& world, Pathfinder& pathfinder,
		const sf::Vector2f& position) :
		Character(position, CATEGORY_ACTOR, MASK_ALL, Yaml(CONFIG), world,
				pathfinder) {
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
		else if (!isMoving())
			setDestination(target->getPosition());
	}
	else
		releaseTrigger();
}
