/*
 * Actor.h
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#ifndef DG_ACTOR_H_
#define DG_ACTOR_H_

#include <vector>

#include "Sprite.h"
#include "../World.h"
#include "../items/Weapon.h"
#include "../types/String.h"
#include "../util/Pathfinder.h"
#include "../util/Yaml.h"

class World;
class Weapon;
class Instances;
class Sprite;
class Pathfinder;
class Yaml;

/**
 * Provides think function for AI, manages health, drops body on death.
 */
class Character : public Sprite {
// Public functions.
public:
	Character(World& world, Collection& collection, Pathfinder& pathfinder,
			const PhysicalData& data, const Yaml& config);
	virtual ~Character() = 0;

	static void think(float elapsedTime);

	void onDamage(int damage);

// Protected functions.
protected:
	virtual void onThink(float elapsedTime);
	virtual void onDeath();
	float getMovementSpeed() const;
	void fire();
	bool setDestination(const Vector2f& destination);
	void move();

// Private variables.
private:
	static const String KEY_HEALTH;
	static const int DEFAULT_HEALTH;
	static const String KEY_SPEED;
	static const float DEFAULT_SPEED;
	/// The distance to a point where it is considered reached.
	static const float POINT_REACHED_DISTANCE;

	static std::vector<Character*> mCharacterInstances;

	Collection& mCollection;
	Pathfinder& mPathfinder;
	World& mWorld;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
	const float mMovementSpeed;
	Weapon mWeapon;
	std::vector<Vector2f> mPath; //< Contains nodes to reach a set destination.
	bool mStartPathfinding; //< True if a movement destination was just set.
};

#endif /* DG_ACTOR_H_ */
