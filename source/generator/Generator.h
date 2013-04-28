/*
 * Generator.h
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#ifndef DG_GENERATOR_H_
#define DG_GENERATOR_H_

#include <SFML/Graphics.hpp>

#include "../sprites/TileManager.h"

class Pathfinder;

class Generator {
public:
	explicit Generator();
	void generateTiles(TileManager& tm, Pathfinder& pathfinder,
			const sf::IntRect& area) const;
	//void generateCharacters(World& world, const sf::IntRect& area) const;
	sf::Vector2f getPlayerSpawn() const;

private:
	static void fill(std::vector<std::vector<TileManager::Type> >& image,
			const sf::IntRect& area, TileManager::Type value);
	static void filterWalls(std::vector<std::vector<TileManager::Type> >& in,
			std::vector<std::vector<TileManager::Type> >& out,
			int x, int y, int longside, int shortside, int subtract);
	static int countWalls(const sf::IntRect& area,
			std::vector<std::vector<TileManager::Type> >& tiles);
	static void generateAreas(Pathfinder& pathfinder,
			std::vector<std::vector<TileManager::Type> >& tiles,
			const sf::IntRect& area, const sf::Vector2f& offset);
};

#endif /* DG_GENERATOR_H_ */
