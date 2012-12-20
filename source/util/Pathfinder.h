/*
 * astar.h
 *
 *  Created on: 24.09.2012
 *      Author: Felix
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <Box2D/Box2D.h>

#include "../abstract/Body.h"
#include "../types/Vector.h"

/**
 * Interface class for Boost Graph A* library.
 */
class Pathfinder {
// Public functions.
public:
	Pathfinder(b2World& world);

	std::vector<Vector2f> getPath(Body& physical, const Vector2f& destination);

// Private variables.
private:
	/// The size of a vertex in pixels. Should be between the size of the moving object
	/// and TileManager::TILE_SIZE.
	static const Vector2f VERTEX_SIZE;

	b2World& mWorld;
};

#endif /* ASTAR_H_ */
