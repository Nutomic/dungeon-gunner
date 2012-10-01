/*
 * Actor.h
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#ifndef DG_ACTOR_H_
#define DG_ACTOR_H_

#include <vector>

/**
 * Provides think function for AI.
 */
class Character {
// Public functions.
public:
	Character(int health);
	virtual ~Character() = 0;

	static void think(float elapsedTime);

	void onDamage(int damage);

// Protected functions.
protected:
	/**
	 * Implement this function for any (regular) AI computations.
	 *
	 * @param elapsedTime Amount of time to simulate.
	 */
	virtual void onThink(float elapsedTime) = 0;

	virtual void onDeath();

// Private variables.
private:
	static std::vector<Character*> mInstances;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
};

#endif /* DG_ACTOR_H_ */
