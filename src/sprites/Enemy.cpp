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
		const Vector2f& position, float seed) :
		Character(position, CATEGORY_ACTOR, MASK_ALL, Yaml("enemy.yaml"), world,
				pathfinder) {
	std::ranlux24_base generator(seed);

	// Select primary weapon.
	switch (std::uniform_int_distribution<int>(0, 4)(generator)) {
	case 0: setFirstWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::ASSAULT_RIFLE));	break;
	case 1:	setFirstWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::AUTO_SHOTGUN));	break;
	case 2: setFirstWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::HMG));			break;
	case 3: setFirstWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::RIFLE));			break;
	case 4: setFirstWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::SHOTGUN));		break;
	}

	// Select secondary weapon.
	switch (std::uniform_int_distribution<int>(0, 1)(generator)) {
	case 0: setSecondWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::PISTOL));	break;
	case 1: setSecondWeapon(Weapon::getWeapon(world, *this, Weapon::WeaponType::KNIFE));	break;
	}

	// Select gadget.
	switch (std::uniform_int_distribution<int>(0, 2)(generator)) {
	case 0: setLeftGadget(std::shared_ptr<Gadget>(new Heal()));				break;
	case 1: setLeftGadget(std::shared_ptr<Gadget>(new Shield()));			break;
	case 2: setLeftGadget(std::shared_ptr<Gadget>(new RingOfFire(world)));	break;
	}

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
