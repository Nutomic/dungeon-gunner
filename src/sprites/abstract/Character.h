/*
 * Actor.h
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#ifndef DG_ACTOR_H_
#define DG_ACTOR_H_

#include "Circle.h"

#include "../items/Gadget.h"

class Pathfinder;
class World;
class Weapon;
class Yaml;

/**
 * Provides think function for AI, manages health, drops body on death.
 */
class Character : public Circle {
public:
	enum Faction {
		FACTION_PLAYER = 1,
		FACTION_ENEMIES = 2
	};

	/// Maximum distance where an enemy will be detected.
	static const float VISION_DISTANCE;
	/// Maximum distance from character where an item can be picked up.
	static const float ITEM_PICKUP_MAX_DISTANCE;

public:
	explicit Character(const Vector2f& position, Category category,
			unsigned short mask, const Yaml& config, World& world,
			Pathfinder& pathfinder);
	virtual ~Character() = 0;

	void onDamage(int damage);
	Faction getFaction() const;

protected:
	virtual void onThink(int elapsed);
	virtual void onDeath();
	float getMovementSpeed() const;
	void pullTrigger();
	void releaseTrigger();
	bool setDestination(const Vector2f& destination);
	bool isPathEmpty() const;
	bool isVisible(const Vector2f& target) const;
	std::vector<std::shared_ptr<Character> > getCharacters() const;
	int getMagazineAmmo() const;
	int getTotalAmmo() const;
	std::string getWeaponName() const;
	void reload();
	void toggleWeapon();
	void selectFirstWeapon();
	void selectSecondWeapon();
	int getHealth() const;
	void setFirstWeapon(std::shared_ptr<Weapon> weapon);
	void setSecondWeapon(std::shared_ptr<Weapon> weapon);
	void setLeftGadget(std::shared_ptr<Gadget> gadget);
	void setRightGadget(std::shared_ptr<Gadget> gadget);
	void useLeftGadget();
	void useRightGadget();
	std::string getLeftGadgetName() const;
	std::string getRightGadgetName() const;
	void pickUpItem();

protected:

private:
	void move();

private:
	friend class Shield;

	/// Distance to a path point where it will be considered as reached (to
	/// avoid floating point equality check).
	static const float POINT_REACHED_DISTANCE;
	friend class World;
	World& mWorld;
	Pathfinder& mPathfinder;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
	const float mMovementSpeed;
	std::shared_ptr<Weapon> mFirstWeapon;
	std::shared_ptr<Weapon> mSecondWeapon;
	std::shared_ptr<Weapon> mActiveWeapon;
	std::shared_ptr<Gadget> mLeftGadget;
	std::shared_ptr<Gadget> mRightGadget;
	std::vector<Vector2f> mPath; //< Contains nodes to reach a set destination.
	Vector2f mLastPosition;
	Faction mFaction;
	bool mIsDead = false;
};

#endif /* DG_ACTOR_H_ */
