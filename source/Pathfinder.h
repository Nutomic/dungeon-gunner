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
	std::vector<sf::Vector2f> getPath(const sf::Vector2f& start,
			const sf::Vector2f& end, float radius) const;

private:
    Area* getArea(const sf::Vector2f& point) const;
    std::vector<Portal*> astarArea(Area* start, Area* end) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

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
	sf::Vector2f start;
	sf::Vector2f end;
	Area* area;
};

/**
 * Nodes
 */
struct Pathfinder::Area {
	sf::FloatRect area;
	sf::Vector2f center;
	std::vector<Portal> portals;
};

#endif /* DG_PATHFINDER_H_ */
