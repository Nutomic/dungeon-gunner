/*
 * Enemy.cpp
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#include "Enemy.h"

#include <Thor/Vectors.hpp>

#include "items/Heal.h"
#include "items/RingOfFire.h"
#include "items/Shield.h"
#include "items/Weapon.h"
#include "../util/Yaml.h"

/**
 * Determines items used by this enemy using seed.
 *
 * @param seed Random value to seed random number generator.
 */
Enemy::Enemy(World& world, Pathfinder& pathfinder,
		const Vector2f& position, const EquippedItems& playerItems) :
		Character(position, CATEGORY_ACTOR, MASK_ALL, Yaml("enemy.yaml"), world,
				pathfinder, generateItems(playerItems)) {
}

/**
 * Returns the items this enemy has equipped, based on items equipped by player.
 *
 * To do this, a random item is replaced by a slightly better one.
 */
Character::EquippedItems
Enemy::generateItems(EquippedItems playerItems) {
	// Uses cast from enum to int to enum in order to increment enum values.
	switch (rand() % 2) {
	case 0:
		if (playerItems.primary + 1 != Weapon::WeaponType::_LAST)
			playerItems.primary =
					(Weapon::WeaponType) (((int) (playerItems.primary) + 1));

		break;
	case 1:
		if (playerItems.secondary + 1 != Weapon::WeaponType::_LAST)
			playerItems.secondary =
					(Weapon::WeaponType) (((int) (playerItems.secondary) + 1));

		break;
	}

	playerItems.left = (Gadget::GadgetType) (rand() % 4);
	playerItems.right = (Gadget::GadgetType) (rand() % 4);

	return playerItems;
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
		else if (isPathEmpty())
			setDestination(target->getPosition());
	}
	else {
		releaseTrigger();
		setSpeed(Vector2f(), 0);
	}
}
