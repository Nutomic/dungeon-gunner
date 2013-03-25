/*
 * Actor.h
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#ifndef DG_ACTOR_H_
#define DG_ACTOR_H_

#include <string>
#include <vector>

#include "Sprite.h"
#include "../World.h"
#include "../items/Weapon.h"
#include "../util/Yaml.h"

class World;
class Weapon;
class Instances;
class Sprite;
class Yaml;

/**
 * Provides think function for AI, manages health, drops body on death.
 */
class Character : public Sprite {
// Public functions.
public:
	Character(World& world, const Data& data, const Yaml& config);
	virtual ~Character() = 0;

	static void think(int elapsed);

	void onDamage(int damage);

// Protected functions.
protected:
	virtual void onThink(int elapsed);
	virtual void onDeath();
	float getMovementSpeed() const;
	void pullTrigger();
	void releaseTrigger();
	bool setDestination(const sf::Vector2f& destination);
	void move();

// Private variables.
private:
	static const std::string KEY_HEALTH;
	static const int DEFAULT_HEALTH;
	static const std::string KEY_SPEED;
	static const float DEFAULT_SPEED;
	static const std::string KEY_WEAPON;
	static const std::string DEFAULT_WEAPON;
	/// The distance to a point where it is considered reached (in pixels).
	static const float POINT_REACHED_DISTANCE;

	static std::vector<Character*> mCharacterInstances;

	World& mWorld;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
	const float mMovementSpeed;
	Weapon mWeapon;
	std::vector<sf::Vector2f> mPath; //< Contains nodes to reach a set destination.
	bool mStartPathfinding; //< True if a movement destination was just set.
};

#endif /* DG_ACTOR_H_ */
