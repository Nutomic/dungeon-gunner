/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Character.h"

#include <Thor/Vectors.hpp>

#include "../items/Weapon.h"
#include "../Corpse.h"
#include "../../util/Log.h"
#include "../../util/Yaml.h"
#include "../../World.h"
#include "../../Pathfinder.h"

const float Character::VISION_DISTANCE = 500.0f;
const float Character::POINT_REACHED_DISTANCE = 1.0f;
const float Character::ITEM_PICKUP_MAX_DISTANCE = 50.0f;

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(const Vector2f& position, Category category,
		unsigned short mask, const Yaml& config, World& world,
		Pathfinder& pathfinder) :
		Circle(position, category, mask, config),
		mWorld(world),
		mPathfinder(pathfinder),
		mMaxHealth(config.get("health", 100)),
		mCurrentHealth(mMaxHealth),
		mMovementSpeed(config.get("speed", 0.0f)),
		mActiveWeapon(mFirstWeapon),
		mLastPosition(getPosition()),
		mFaction((Faction) config.get("faction", 1)) {
}

Character::~Character() {
}

/**
 * Subtracts health from Actor. Calls onDeath() when health reaches zero and marks
 * object for deletion.
 *
 * @param damage Amount of damage taken.
 */
void
Character::onDamage(int damage) {
	mCurrentHealth -= damage;


	if (mCurrentHealth > mMaxHealth)
		mCurrentHealth = mMaxHealth;

	if (mCurrentHealth <= 0) {
		// Seperated to avoid firing multiple times (when damaged multiple times).
		if (!mIsDead)
			onDeath();
		mIsDead = true;
		mCurrentHealth = 0;
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
	if (mCurrentHealth == 0)
		return;

	mActiveWeapon->onThink(elapsed);
	if (mLeftGadget)
		mLeftGadget->onThink(elapsed);
	if (mRightGadget)
		mRightGadget->onThink(elapsed);
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
 * Called when health reaches zero. Drops corpse and item.
 */
void
Character::onDeath() {
	mWorld.insert(std::shared_ptr<Sprite>(new Corpse(getPosition())));

	mWorld.insert(mActiveWeapon);
	mActiveWeapon->drop(getPosition());
	// To avoid the weapon continuing to fire after pickup.
	mActiveWeapon->releaseTrigger();

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
	mActiveWeapon->pullTrigger();
}

/**
 * Release the trigger on the attached weapon.
 */
void
Character::releaseTrigger() {
	mActiveWeapon->releaseTrigger();
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
Character::setDestination(const Vector2f& destination) {
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
			setSpeed(Vector2f(), 0);
	}
}

/**
 * Returns true if the path is empty.
 */
bool
Character::isPathEmpty() const {
	return mPath.empty();
}

/**
 * Tests if a target is visible from the current position.
 */
bool
Character::isVisible(const Vector2f& target) const {
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
	return mActiveWeapon->getMagazineAmmo();
}

int
Character::getTotalAmmo() const {
	return mActiveWeapon->getTotalAmmo();
}

std::string
Character::getWeaponName() const {
	return mActiveWeapon->getName();
}

void
Character::reload() {
	mActiveWeapon->reload();
}

void
Character::toggleWeapon() {
	mActiveWeapon->cancelReload();
	mActiveWeapon = (mActiveWeapon == mFirstWeapon)
		? mSecondWeapon
		: mFirstWeapon;
}

void
Character::selectFirstWeapon() {
	mActiveWeapon->cancelReload();
	mActiveWeapon = mFirstWeapon;
}

void
Character::selectSecondWeapon() {
	mActiveWeapon->cancelReload();
	mActiveWeapon = mSecondWeapon;
}

/**
 * Returns current player health.
 */
int
Character::getHealth() const {
	return mCurrentHealth;
}

/**
 * Sets first weapon to weapon, also replacing active weapon if first weapon
 * is active.
 */
void
Character::setFirstWeapon(std::shared_ptr<Weapon> weapon) {
	if (mFirstWeapon == mActiveWeapon)
		mActiveWeapon = weapon;
	mFirstWeapon = weapon;
}

/**
 * Sets second weapon to weapon, also replacing active weapon if second weapon
 * is active.
 */
void
Character::setSecondWeapon(std::shared_ptr<Weapon> weapon) {
	if (mSecondWeapon == mActiveWeapon)
		mActiveWeapon = weapon;
	mSecondWeapon = weapon;
}

void
Character::setLeftGadget(std::shared_ptr<Gadget> gadget) {
	mLeftGadget = gadget;
}

void
Character::setRightGadget(std::shared_ptr<Gadget> gadget) {
	mRightGadget = gadget;
}

void
Character::useLeftGadget() {
	mLeftGadget->use(*this);
}

void
Character::useRightGadget() {
	mRightGadget->use(*this);
}

std::string
Character::getLeftGadgetName() const {
	return mLeftGadget->getName();
}

std::string
Character::getRightGadgetName() const {
	return mRightGadget->getName();
}

/**
 * Picks up the nearest weapon or gadget, and drops the active weapon (or right gadget).
 *
 * If no item is nearby, gadgets are swapped instead.
 */
void
Character::pickUpItem() {
	std::shared_ptr<Item> closest = mWorld.getClosestItem(getPosition());

	if (closest == nullptr) {
		std::swap(mLeftGadget, mRightGadget);
		return;
	}

	std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(closest);
	if (weapon != nullptr) {
		mWorld.insert(mActiveWeapon);
		mActiveWeapon->drop(getPosition());
		mActiveWeapon = weapon;
		mActiveWeapon->setHolder(*this);
	}
	std::shared_ptr<Gadget> gadget = std::dynamic_pointer_cast<Gadget>(closest);
	if (gadget != nullptr) {
		mWorld.insert(mRightGadget);
		mRightGadget->drop(getPosition());
		mRightGadget = mLeftGadget;
		mLeftGadget = gadget;
	}
	mWorld.remove(closest);
}
