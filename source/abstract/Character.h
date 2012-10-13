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
#include "../items/Weapon.h"
#include "../util/Instances.h"
#include "../util/String.h"
#include "../util/Yaml.h"

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
	Character(const Instances& instances, const String& texturePath,
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

// Private variables.
private:
	static const String KEY_HEALTH;
	static const String KEY_SPEED;

	static std::vector<Character*> mCharacterInstances;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
	const float mMovementSpeed;
	Weapon mWeapon;
	Instances mInstances;
};

#endif /* DG_ACTOR_H_ */
