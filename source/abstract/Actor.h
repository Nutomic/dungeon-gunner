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
class Actor {
// Public functions.
public:
	Actor();
	virtual ~Actor() = 0;

	static void think(float elapsedTime);

// Protected functions.
protected:
	/**
	 * Implement this function for any (regular) AI computations.
	 *
	 * @param elapsedTime Amount of time to simulate.
	 */
	virtual void onThink(float elapsedTime) = 0;

// Private variables.
private:
	static std::vector<Actor*> mInstances;
};

#endif /* DG_ACTOR_H_ */
