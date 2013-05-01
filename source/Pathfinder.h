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

class Pathfinder {
private:
	struct Area;
	struct Portal;

public:
	void insertArea(const sf::IntRect& rect);
	void generatePortals();
	std::vector<sf::Vector2f> getPath(const sf::Vector2f& start,
			const sf::Vector2f& end, float radius) const;

private:
    Area* getArea(const sf::Vector2f& point) const;
    std::vector<Portal*> astarArea(Area* start, Area* end) const;

private:
    static const float WALL_DISTANCE_MULTIPLIER;
	std::vector<Area> mAreas; //< This has to be a vector as objects are compared by address.
};

/**
 * Edges
 *
 * Redundant data as portals are saved twice.
 */
struct Pathfinder::Portal {
	sf::Vector2i start;
	sf::Vector2i end;
	Area* area;
};

/**
 * Nodes
 */
struct Pathfinder::Area {
	sf::IntRect area;
	sf::Vector2i center;
	std::vector<Portal> portals;
};

#endif /* DG_PATHFINDER_H_ */
