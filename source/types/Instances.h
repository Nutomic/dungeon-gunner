/*
 * Instances.h
 *
 *  Created on: 04.10.2012
 *      Author: Felix
 */

#ifndef DG_INSTANCES_H_
#define DG_INSTANCES_H_

#include <Box2D/Box2D.h>

#include "../sprites/TileManager.h"
#include "../util/Collection.h"
#include "../util/Pathfinder.h"

class Pathfinder;
class TileManager;
class Collection;

/**
 * POD class that holds instances of major classes used by other objects.
 */
struct Instances {
	Instances() = default;
	Instances(Pathfinder& p, TileManager& t, Collection& c, b2World& w) :
			pathfinder(p), tilemanager(t), collection(c), world(w) {};

	Pathfinder& pathfinder;
	TileManager& tilemanager;
	Collection& collection;
	b2World& world;
};

#endif /* DG_INSTANCES_H_ */
