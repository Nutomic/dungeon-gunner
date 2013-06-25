/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Character.h"

#include <Thor/Vectors.hpp>

#include "../items/Weapon.h"
#include "../sprites/Corpse.h"
#include "../util/Log.h"
#include "../util/Yaml.h"
#include "../World.h"
#include "../Pathfinder.h"

const float Character::VISION_DISTANCE = 500.0f;
const float Character::POINT_REACHED_DISTANCE = 1.0f;

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(const sf::Vector2f& position, Category category,
		unsigned short mask, const Yaml& config, World& world,
		Pathfinder& pathfinder) :
		Circle(position, category, mask, config),
		mWorld(world),
		mPathfinder(pathfinder),
		mMaxHealth(config.get("health", 100)),
		mCurrentHealth(mMaxHealth),
		mMovementSpeed(config.get("speed", 0.0f)),
		mWeapon(new Weapon(world, *this,
				Yaml(config.get("weapon", std::string())))),
		mLastPosition(getPosition()),
		mFaction((Faction) config.get("faction", 1)) {
}

Character::~Character() {
}

/**
 * Subtracts health from Actor. Calls onDeath() when health reaches zero and marks
 * object for deletion.
 *
 * @param damage Amount of health to subtract.
 */
void
Character::onDamage(int damage) {
	mCurrentHealth -= damage;
	if (mCurrentHealth <= 0) {
		mCurrentHealth = 0;
		onDeath();
		setDelete(true);
	}
}

/**
 * Implement this function for any (periodical) AI computations.
 * If overwritten, this function should always be called from the overwriting function.
 *
 * @param elapsed Amount of time to simulate.
 */
void
Character::onThink(int elapsed) {
	mWeapon->onThink(elapsed);
	move();
}

/**
 * Returns the faction this character belongs to.
 */
Character::Faction
Character::getFaction() const {
	return mFaction;
}

/**
 * Called when health reaches zero. Default implementation drops a corpse at
 * the current position.
 */
void
Character::onDeath() {
	mWorld.insert(std::shared_ptr<Sprite>(new Corpse(getPosition())));
}

/**
 * Gets the default movement speed (walking) of the character.
 */
float
Character::getMovementSpeed() const {
	return mMovementSpeed;
}

/**
 * Pull the trigger on the attached weapon.
 */
void
Character::pullTrigger() {
	mWeapon->pullTrigger();
}

/**
 * Release the trigger on the attached weapon.
 */
void
Character::releaseTrigger() {
	mWeapon->releaseTrigger();
}

/**
 * Set a destination to be walked to. Call move() to actually
 * perform the movement.
 *
 * @param destination An absolute point to move towards. Set to current
 * 						position to stop movement.
 * @return True if a path was found.
 */
bool
Character::setDestination(const sf::Vector2f& destination) {
	mPath = mPathfinder.getPath(getPosition(), destination, getRadius());
	return !mPath.empty();
}

/**
 * Move towards a destination. Call setDestination() for setting the destination.
 * This is automatically called from onThink().
 */
void
Character::move() {
	if (mPath.empty())
		return;
	mLastPosition = getPosition();
	setSpeed(mPath.back() - getPosition(), mMovementSpeed);
	setDirection(mPath.back() - getPosition());
	if (thor::length(mPath.back() - getPosition()) < POINT_REACHED_DISTANCE) {
		mPath.pop_back();
		if (mPath.empty())
			setSpeed(sf::Vector2f(), 0);
	}
}

/**
 * Returns true if the character is currently moving.
 */
bool
Character::isMoving() const {
	return !mPath.empty();
}

/**
 * Tests if a target is visible from the current position.
 */
bool
Character::isVisible(const sf::Vector2f& target) const {
	return mWorld.raycast(getPosition(), target);
}

/**
 * Calls World::getCharacters with current position.
 */
std::vector<std::shared_ptr<Character> >
Character::getCharacters() const {
	auto characters = mWorld.getCharacters(getPosition(), VISION_DISTANCE);
	characters.erase(std::remove_if(characters.begin(), characters.end(),
			[this](const std::shared_ptr<Character>& c)
			{return c->getFaction() == getFaction();}), characters.end());
	return characters;
}

int
Character::getMagazineAmmo() const {
	return mWeapon->getMagazineAmmo();
}

int
Character::getTotalAmmo() const {
	return mWeapon->getTotalAmmo();
}

void
Character::reload() {
	mWeapon->reload();
}
