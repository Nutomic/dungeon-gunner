/*
 * Generator.h
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#ifndef DG_GENERATOR_H_
#define DG_GENERATOR_H_

#include <SFML/Graphics.hpp>

#include "../sprites/Tile.h"

class World;
class Pathfinder;

class Generator {
public:
	explicit Generator(World& world, Pathfinder& pathfinder);
	void generateTiles(const sf::IntRect& area);
	sf::Vector2f getPlayerSpawn() const;

private:
	typedef Tile::Type type;
	typedef std::map<int, std::map<int, type> > array;

private:
	sf::Vector2i findClosestFloor(const sf::Vector2i& position) const;
	static void fill(array& in, const sf::IntRect& area, type value);
	static void filterWalls(const array& in, array& out, int x, int y,
			int longside, int shortside, int subtract);
	static int countWalls(const array& in, const sf::IntRect& area);
	void generateAreas(const array& in, const sf::IntRect& area,
			const sf::Vector2f& offset) const;

private:
	static const int MARGIN;
	World& mWorld;
	Pathfinder& mPathfinder;
	array mGenerated;
};

#endif /* DG_GENERATOR_H_ */
