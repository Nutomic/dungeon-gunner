/*
 * pathfinder.h
 *
 *  Created on: 24.09.2012
 *      Author: Felix
 */

#ifndef PATHFINDER_H_
#define PATHFINDER_H_

#include <Box2D/Box2D.h>

#include "../abstract/Body.h"
#include "../types/Vector.h"

class Body;

class Pathfinder {
// Public functions.
public:
	Pathfinder();

	std::vector<Vector2f> getPath(Body& physical, const Vector2f& destination);
};

#endif /* PATHFINDER_H_ */
