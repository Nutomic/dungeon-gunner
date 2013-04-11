/*
 * Generator.h
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#ifndef DG_GENERATOR_H_
#define DG_GENERATOR_H_

#include <SFML/Graphics.hpp>

class TileManager;

class Generator {
public:
	explicit Generator();
	void generateTiles(TileManager& tm, const sf::IntRect& area) const;
	//void generateCharacters(World& world, const sf::IntRect& area) const;
	sf::Vector2f getPlayerSpawn() const;

private:
	static void fill(std::vector<std::vector<bool> >& image,
			const sf::IntRect& area, bool value);
	static void filterWalls(std::vector<std::vector<bool> >& in,
			std::vector<std::vector<bool> >& out,
			int x, int y, int longside, int shortside, int subtract);
};

#endif /* DG_GENERATOR_H_ */
