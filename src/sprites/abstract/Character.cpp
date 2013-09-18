/*
 * Actor.cpp
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#include "Character.h"

#include <Thor/Vectors.hpp>

#include "../items/HealthOrb.h"
#include "../items/Weapon.h"
#include "../Corpse.h"
#include "../../util/Log.h"
#include "../../util/Yaml.h"
#include "../../World.h"
#include "../../Pathfinder.h"

/**
 * Saves pointer to this instance in static var for think().
 */
Character::Character(const Vector2f& position, Category category,
		unsigned short mask, const Yaml& config, World& world,
		Pathfinder& pathfinder, const EquippedItems& items) :
		Circle(position, category, mask, config),
		mWorld(world),
		mPathfinder(pathfinder),
		mMaxHealth(config.get("health", 100)),
		mHealth(mMaxHealth),
		mMovementSpeed(config.get("speed", 0.0f)),
		mActiveWeapon(mFirstWeapon),
		mLastPosition(getPosition()),
		mFaction((Faction) config.get("faction", 1)) {
	 setFirstWeapon(Weapon::getWeapon(world, *this, items.primary));
	 setSecondWeapon(Weapon::getWeapon(world, *this, items.secondary));
	 setLeftGadget(Gadget::getGadget(world, items.left));
	 setRightGadget(Gadget::getGadget(world, items.right));
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
	// Otherwise player might not respawn after death
	if (mHealth <= 0)
		return;

	mHealth -= damage;

	if (mHealth > mMaxHealth)
		mHealth = mMaxHealth;

	if (mHealth <= 0) {
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
	if (mHealth <= 0)
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
 * Inserts the item into the world. Does not remove it from the Character.
 */
void Character::dropItem(std::shared_ptr<Item> item) {
	if (!item)
		return;

	// To avoid weapons continuing to fire after drop and pickup.
	auto weapon = std::dynamic_pointer_cast<Weapon>(item);
	if (weapon)
		weapon->releaseTrigger();

	mWorld.insert(item);
	item->drop(getPosition());
}

/**
 * Called when health reaches zero, drops corpse and a random item.
 */
void
Character::onDeath() {
	mWorld.insert(std::shared_ptr<Sprite>(new Corpse(getPosition())));

	if (rand() % 2)
		dropItem(std::make_shared<HealthOrb>());
	else
		switch (rand() % 3) {
		case 0:
			dropItem(mFirstWeapon);
			break;
		case 1:
			dropItem(mSecondWeapon);
			break;
		case 2:
			dropItem(mLeftGadget);
			break;
		case 3:
			dropItem(mRightGadget);
			break;
		}
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
	(mActiveWeapon == mFirstWeapon)
		? selectSecondWeapon()
		: selectFirstWeapon();
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
 * Returns current player health. A value <= 0 means this character is dead.
 */
int
Character::getHealth() const {
	return mHealth;
}

int
Character::getMaxHealth() const {
	return mMaxHealth;
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
	mFirstWeapon->setHolder(*this);
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
	mSecondWeapon->setHolder(*this);
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
	if (mLeftGadget)
		mLeftGadget->use(*this);
}

void
Character::useRightGadget() {
	if (mRightGadget)
		mRightGadget->use(*this);
}

std::string
Character::getLeftGadgetName() const {
	return (mLeftGadget)
			? mLeftGadget->getName()
			: "";
}

std::string
Character::getRightGadgetName() const {
	return (mRightGadget)
			? mRightGadget->getName()
			: "";
}

/**
 * Picks up the nearest weapon or gadget, and drops the active weapon (or right gadget).
 *
 * If no item is nearby, gadgets are swapped instead.
 */
void
Character::pickUpItem() {
	std::shared_ptr<Item> closest = mWorld.getClosestItem(getPosition());

	if (!closest) {
		std::swap(mLeftGadget, mRightGadget);
		return;
	}

	auto weapon = std::dynamic_pointer_cast<Weapon>(closest);
	auto gadget = std::dynamic_pointer_cast<Gadget>(closest);
	auto orb = std::dynamic_pointer_cast<HealthOrb>(closest);

	if (weapon) {
		mWorld.insert(mActiveWeapon);
		mActiveWeapon->drop(getPosition());
		(mActiveWeapon == mFirstWeapon)
				? setFirstWeapon(weapon)
				: setSecondWeapon(weapon);
	}
	else if (gadget) {
		if (mRightGadget) {
			mWorld.insert(mRightGadget);
			mRightGadget->drop(getPosition());
		}
		mRightGadget = mLeftGadget;
		mLeftGadget = gadget;
	}
	else if (orb) {
		onDamage(- orb->getAmountHealed());
	}
	mWorld.remove(closest);
}

Character::EquippedItems
Character::getEquippedItems() const {
	return {
		(mFirstWeapon) ? mFirstWeapon->getType() : Weapon::WeaponType::NONE,
		(mSecondWeapon) ? mSecondWeapon->getType() : Weapon::WeaponType::NONE,
		(mLeftGadget) ? mLeftGadget->getType() : Gadget::GadgetType::NONE,
		(mRightGadget) ? mRightGadget->getType() : Gadget::GadgetType::NONE
	};
}
