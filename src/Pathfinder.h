/*
 * Pathfinder.h
 *
 *  Created on: 28.04.2013
 *      Author: Felix
 */

#ifndef DG_PATHFINDER_H_
#define DG_PATHFINDER_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "util/Vector.h"

/**
 * Used to find paths between points in the world.
 *
 * For this, a representation of all walkable areas in the world is
 * saved, which is then used to run A* on.
 */
class Pathfinder : public sf::Drawable {
private:
	struct Area;
	struct Portal;

public:
	void insertArea(const sf::FloatRect& rect);
	void generatePortals();
	std::vector<Vector2f> getPath(const Vector2f& start,
			const Vector2f& end, float radius) const;

private:
    Area* getArea(const Vector2f& point) const;
    std::vector<Portal*> astarArea(Area* start, Area* end) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	/// character radius multiplied with this gives movement distance
	// from the nearest wall.
    static constexpr float WALL_DISTANCE_MULTIPLIER = 1.5f;
    //< This has to be a vector as objects are compared by address.
	std::vector<Area> mAreas;
};

/**
 * Edges
 *
 * Redundant data as portals are saved twice.
 */
struct Pathfinder::Portal {
	Vector2f start;
	Vector2f end;
	Area* area;
};

/**
 * Nodes
 */
struct Pathfinder::Area {
	sf::FloatRect area;
	Vector2f center;
	std::vector<Portal> portals;
};

#endif /* DG_PATHFINDER_H_ */
