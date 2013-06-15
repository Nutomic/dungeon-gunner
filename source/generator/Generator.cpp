/*
 * Generator.cpp
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#include "Generator.h"

#include <algorithm>
#include <assert.h>
#include <map>
#include <set>

#include <SFML/System.hpp>

#include <Thor/Vectors.hpp>

#include "../Pathfinder.h"
#include "../World.h"
#include "../util/Log.h"

/// Seed for usage with simplexnoise.h
uint8_t perm[512];

const int Generator::GENERATE_AREA_SIZE = 4;
const float Generator::GENERATE_AREA_RANGE = 4.0f;

/**
 * Amount of tiles extra to generate, to get consistent walls
 * across multiple generateTiles calls for bordering areas.
 */
const int Generator::MARGIN = 10;

// Different layers in 3d noise so we don't use the same noise values.
const float Generator::LAYER_TILES = 0;
const float Generator::LAYER_ENEMIES = 1.0f;

/**
 * Generates new random seed.
 */
Generator::Generator(World& world, Pathfinder& pathfinder) :
		mWorld(world),
		mPathfinder(pathfinder) {
}

/**
 * Generates tiles near position (maximum distance is determined by
 * GENERATE_AREA_SIZE and GENERATE_AREA_RANGE).
 */
void
Generator::generateCurrentAreaIfNeeded(const sf::Vector2f& position) {
	auto compare = [](const sf::Vector2i& a, const sf::Vector2i& b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	};
	std::map<sf::Vector2i, float, decltype(compare)> open(compare);
	std::set<sf::Vector2i, decltype(compare)> closed(compare);

	sf::Vector2i start((int) floor(position.x / Tile::TILE_SIZE.x),
			(int) floor(position.y / Tile::TILE_SIZE.y));
	start /= GENERATE_AREA_SIZE;
	auto makePair = [&start](const sf::Vector2i& point) {
		return std::make_pair(point, thor::length(sf::Vector2f(point - start)));
	};

	open.insert(makePair(start));
	while (!open.empty()) {
		auto intComp = [](const std::pair<sf::Vector2i, float>& left,
				const std::pair<sf::Vector2i, float>& right) {
			return left.second < right.second;
		};
		sf::Vector2i current =
				std::min_element(open.begin(), open.end(), intComp)->first;
		float distance = open[current];
		open.erase(current);
		closed.insert(current);
		if (!mGenerated[current.x][current.y] && distance <= GENERATE_AREA_RANGE) {
			mGenerated[current.x][current.y] = true;
			sf::IntRect area(current * GENERATE_AREA_SIZE -
					sf::Vector2i(GENERATE_AREA_SIZE, GENERATE_AREA_SIZE) / 2,
					sf::Vector2i(GENERATE_AREA_SIZE, GENERATE_AREA_SIZE));
			LOG_I("Generating area " << area);
			generateTiles(area);
		}
		if (mGenerated[current.x][current.y] && distance <= GENERATE_AREA_RANGE) {
			if (closed.find(sf::Vector2i(current.x + 1, current.y)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x + 1, current.y)));
			if (closed.find(sf::Vector2i(current.x, current.y + 1)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x, current.y + 1)));
			if (closed.find(sf::Vector2i(current.x - 1, current.y)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x - 1, current.y)));
			if (closed.find(sf::Vector2i(current.x, current.y - 1)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x, current.y - 1)));
		}
	}
}

/**
 * Fill world with procedurally generated tiles.
 *
 * @param area Size and position of area to generate tiles for. Width and
 * 				height must each be a power of two.
 */
void
Generator::generateTiles(const sf::IntRect& area) {
	// Check if width and height are power of two.
	assert(area.width && !(area.width & (area.width - 1)));
	assert(area.height && !(area.height & (area.height - 1)));

	array generatedTiles;
	fill(generatedTiles, area, type::FLOOR);

	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			filterWalls(generatedTiles, x, y, 2, 1, 0);
			filterWalls(generatedTiles, x, y, 6, 1, 2);
			filterWalls(generatedTiles, x, y, 10, 1, 4);
		}
	}

	for (int x = area.left; x < area.left + area.width; x++)
		for (int y = area.top; y < area.top + area.height; y++) {
			// Merge map that we just generated with stored map.
			mTiles[x][y] = generatedTiles[x][y];
			// Actually generate physical tiles.
			mWorld.insert(std::shared_ptr<Sprite>(
					new Tile(generatedTiles.at(x).at(y), x, y)));
		}

	generateAreas(area, sf::Vector2f(area.left, area.top));
	mPathfinder.generatePortals();
}

std::vector<sf::Vector2f>
Generator::getEnemySpawns(const sf::IntRect& area) {
	auto compare = [](const sf::Vector2f& a, const sf::Vector2f& b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	};
	std::set<sf::Vector2f, decltype(compare)> ret(compare);
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			if (mCharacterNoise.getNoise(x, y) < 0.05f) {
				ret.insert(sf::Vector2f(thor::cwiseProduct(
						findClosestFloor(sf::Vector2i(x, y)), Tile::TILE_SIZE)));
			}
		}
	}
	return std::vector<sf::Vector2f>(ret.begin(), ret.end());
}

/**
 * Fills a rectangular area with the specified value.
 *
 * @param[in,out] Array to set values to.
 * @param area The area to fill.
 * @param value The value to set.
 */
void
Generator::fill(array& tiles, const sf::IntRect& area, Tile::Type value) {
	for (int x = area.left;	x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++)
			tiles[x][y] = value;
	}
}

/**
 * Counts and returns the number of walls within the area.
 *
 * @param area The area to count in.
 */
int
Generator::countWalls(const sf::IntRect& area) {
	int count = 0;
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++)
			count += (int) (getTileType(mCharacterNoise.getNoise(x, y)) ==
					type::WALL);
	}
	return count;
}

/**
 * Finds rectangles of specific size with mTileNoise and
 * puts them into vector out.
 *
 * @param[in,out] tiles Tiles to be placed. Does not explicitly set floor values
 * 						(keeps previous values).
 * @param x Position to check from (top left corner for rectangle).
 * @param y Position to check from (top left corner for rectangle).
 * @param longside Length of the longer side of the rectangle.
 * @param shortside Length of the shorter side of the rectangle.
 * @param subtract Still accepts rectangle if at least this amount of
 * 			tiles is not walls (tilecount >= longside * shortside - subtract).
 */
void
Generator::filterWalls(array& tiles, int x, int y, int longside,
		int shortside, int subtract) {
	// Filter in horizontal direction.
	if (countWalls(sf::IntRect(x, y, longside, shortside)) >=
			shortside * longside - subtract)
		fill(tiles, sf::IntRect(x, y, longside, shortside), type::WALL);
	// Filter in vertical direction.
	if (countWalls(sf::IntRect(x, y, shortside, longside)) >=
			shortside * longside - subtract)
		fill(tiles, sf::IntRect(x, y, shortside, longside), type::WALL);
}

/**
 * Inserts floor tiles into path finder, using a quadtree approach to group
 * tiles where possible.
 *
 * @param area The area to generate areas for.
 * @param offset Offset of tiles[0][0] from World coordinate (0, 0).
 */
void
Generator::generateAreas(const sf::IntRect& area,
		const sf::Vector2f& offset) {
	assert(area.width > 0 && area.height > 0);
	int count = countWalls(area);
	if (count == 0) {
		mPathfinder.insertArea(sf::IntRect(area));
	}
	else if (count == area.width * area.height) {
		return;
	}
	else {
		int halfWidth = area.width / 2.0f;
		int halfHeight = area.height / 2.0f;
		generateAreas(sf::IntRect(area.left,
				area.top,             halfWidth, halfHeight), offset);
		generateAreas(sf::IntRect(area.left + halfWidth,
				area.top,             halfWidth, halfHeight), offset);
		generateAreas(sf::IntRect(area.left,
				area.top + halfHeight, halfWidth, halfHeight), offset);
		generateAreas(sf::IntRect(area.left + halfWidth,
				area.top + halfHeight, halfWidth, halfHeight), offset);
	}
}

/**
 * Defines if a perlin noise result value is converted to a wall or floor tile.
 *
 * @param value Perlin noise value within [-1, 1]
 */
Generator::type
Generator::getTileType(float value) {
	return (value < -0.2f)
			? type::WALL
			: type::FLOOR;
}

/**
 * Returns a valid position (floor) for the player to spawn at.
 */
sf::Vector2f
Generator::getPlayerSpawn() const {
	sf::Vector2i spawn = findClosestFloor(sf::Vector2i(0, 0));
	return sf::Vector2f(spawn.x * Tile::TILE_SIZE.x,
						spawn.y * Tile::TILE_SIZE.y);
}

/**
 * Finds the point array index closest to position which has a floor tile.
 *
 * @position Point to start search for a floor tile from.
 */
sf::Vector2i
Generator::findClosestFloor(const sf::Vector2i& position) const {
	auto compare = [](const sf::Vector2i& a, const sf::Vector2i& b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	};
	std::map<sf::Vector2i, float, decltype(compare)> open(compare);
	std::set<sf::Vector2i, decltype(compare)> closed(compare);
	sf::Vector2i start = position;
	auto makePair = [&start](const sf::Vector2i& point) {
		return std::make_pair(point, thor::length(sf::Vector2f(point - start)));
	};

	open.insert(makePair(start));
	while (!open.empty()) {
		auto intComp = [](const std::pair<sf::Vector2i, float>& left,
				const std::pair<sf::Vector2i, float>& right) {
			return left.second < right.second;
		};
		sf::Vector2i current = std::min_element(open.begin(), open.end(), intComp)->first;
		open.erase(current);
		closed.insert(current);
		if (mTiles.count(current.x) != 0 &&
				mTiles.at(current.x).count(current.y) != 0 &&
				mTiles.at(current.x).at(current.y) == Tile::Type::FLOOR)
			return current;
		else {
			if (closed.find(sf::Vector2i(current.x + 1, current.y)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x + 1, current.y)));
			if (closed.find(sf::Vector2i(current.x, current.y + 1)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x, current.y + 1)));
			if (closed.find(sf::Vector2i(current.x - 1, current.y)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x - 1, current.y)));
			if (closed.find(sf::Vector2i(current.x, current.y - 1)) == closed.end())
				open.insert(makePair(sf::Vector2i(current.x, current.y - 1)));
		}
	}
	assert(false);
	return sf::Vector2i();
}
