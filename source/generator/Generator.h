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
	sf::Vector2i findClosestFloor(const sf::Vector2i& position) const;
	static void fill(std::vector<std::vector<Tile::Type> >& image,
			const sf::IntRect& area, Tile::Type value);
	static void filterWalls(std::vector<std::vector<Tile::Type> >& in,
			std::vector<std::vector<Tile::Type> >& out,
			int x, int y, int longside, int shortside, int subtract);
	static int countWalls(const sf::IntRect& area,
			std::vector<std::vector<Tile::Type> >& tiles);
	void generateAreas(std::vector<std::vector<Tile::Type> >& tiles,
			const sf::IntRect& area, const sf::Vector2f& offset);

private:
	World& mWorld;
	Pathfinder& mPathfinder;
	std::vector<std::vector<Tile::Type> > mGenerated;
};

#endif /* DG_GENERATOR_H_ */
