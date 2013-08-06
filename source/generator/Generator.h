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
#include "SimplexNoise.h"

class World;
class Pathfinder;

/**
 * Procedurally generates tiles, chooses player and enemy spawn positions.
 */
class Generator {
public:
	explicit Generator(World& world, Pathfinder& pathfinder);
	void generateCurrentAreaIfNeeded(const sf::Vector2f& position);
	sf::Vector2f getPlayerSpawn() const;
	std::vector<sf::Vector2f> getEnemySpawns(const sf::IntRect& area);

private:
	typedef std::map<int, std::map<int, Tile::Type> > array;

private:
	void generateAreas(const sf::IntRect& area);
	void generateTiles(const sf::IntRect& area);
	sf::Vector2i findClosestFloor(const sf::Vector2i& position) const;
	std::vector<sf::Vector2i> createMinimalSpanningTree(
			const sf::Vector2i& start, const float limit);

private:
	static const int GENERATE_AREA_SIZE;
	static const float GENERATE_AREA_RANGE;

	World& mWorld;
	Pathfinder& mPathfinder;
	/// Contains values of all tiles that have yet been generated.
	array mTiles;
	/// Stores where tiles have already been generated.
	std::map<int, std::map<int, bool> > mGenerated;
	/// Perlin noise used for tile generation.
	SimplexNoise mTileNoise;
	/// Perlin noise used for character placement.
	SimplexNoise mCharacterNoise;
};

#endif /* DG_GENERATOR_H_ */
