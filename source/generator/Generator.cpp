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

#include "simplexnoise.h"
#include "../Pathfinder.h"
#include "../World.h"

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
	std::mt19937 mersenne(time(nullptr));
	std::uniform_int_distribution<int> distribution(0, 255);

	for (int i = 0; i < 512; i++)
		perm[i] = distribution(mersenne);
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
			generateTiles(sf::IntRect(current * GENERATE_AREA_SIZE -
					sf::Vector2i(GENERATE_AREA_SIZE, GENERATE_AREA_SIZE) / 2,
					sf::Vector2i(GENERATE_AREA_SIZE, GENERATE_AREA_SIZE)));
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

	array noise;
	array filtered;
	for (int x = area.left - MARGIN; x < area.left + area.width + MARGIN; x++) {
		for (int y = area.top - MARGIN; y < area.top + area.height + MARGIN; y++) {
			noise[x][y] =
					(scaled_octave_noise_3d(2, 2, 0.05f, 0.5f, -0.5f, x, y, LAYER_TILES) +
					scaled_octave_noise_3d(2, 2, 0.5f, 0.15f, -0.15f, x, y, LAYER_TILES)
					< -0.1f)
							? type::WALL
							: type::FLOOR;
		}
	}
	fill(filtered, area, type::FLOOR);

	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			filterWalls(noise, filtered, x, y, 2, 1, 0);
			filterWalls(noise, filtered, x, y, 6, 1, 2);
			filterWalls(noise, filtered, x, y, 10, 1, 4);
		}
	}

	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			mWorld.insert(std::shared_ptr<Sprite>(
					new Tile(filtered.at(x).at(y), x, y)));
		}
	}
	generateAreas(filtered, area, sf::Vector2f(area.left, area.top));
	mPathfinder.generatePortals();
	mTiles = filtered;
}

std::vector<sf::Vector2f>
Generator::getEnemySpawns(const sf::IntRect& area) const {
	auto compare = [](const sf::Vector2f& a, const sf::Vector2f& b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	};
	std::set<sf::Vector2f, decltype(compare)> ret(compare);
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			if (scaled_octave_noise_3d(2, 2, 0.5f, 10.0f, 0, x, y, LAYER_ENEMIES)
			< 1.0f) {
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
Generator::fill(array& image, const sf::IntRect& area, Tile::Type value) {
	for (int x = area.left;	x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++)
			image[x][y] = value;
	}
}

/**
 * Counts and returns the number of walls within the area.
 *
 * @param[in] tiles Array of tile values.
 * @param area The area to count in.
 */
int
Generator::countWalls(const array& tiles, const sf::IntRect& area) {
	int count = 0;
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++)
			count += (int) (tiles.at(x).at(y) == type::WALL);
	}
	return count;
}

/**
 * Finds rectangles of specific size inside vector in and
 * puts them into vector out.
 *
 * @param[in] in Perlin noise values.
 * @param[in,out] out Tiles to be placed. Does not explicitly set floor values
 * 						(keeps previous values).
 * @param x Position to check from (top left corner for rectangle).
 * @param y Position to check from (top left corner for rectangle).
 * @param longside Length of the longer side of the rectangle.
 * @param shortside Length of the shorter side of the rectangle.
 * @param subtract Still accepts rectangle if at least this amount of
 * 			tiles is not walls (tilecount >= longside * shortside - subtract).
 */
void
Generator::filterWalls(const array& in, array& out, int x, int y, int longside,
		int shortside, int subtract) {
	// Filter in horizontal direction.
	if (countWalls(in, sf::IntRect(x, y, longside, shortside)) >=
			shortside * longside - subtract)
		fill(out, sf::IntRect(x, y, longside, shortside), type::WALL);
	// Filter in vertical direction.
	if (countWalls(in, sf::IntRect(x, y, shortside, longside)) >=
			shortside * longside - subtract)
		fill(out, sf::IntRect(x, y, shortside, longside), type::WALL);
}

/**
 * Inserts tile if all values within area are the same, otherwise divides area
 * into four and continues recursively.
 *
 * @param in Array of tile values.
 * @param area The area to generate areas for.
 * @param offset Offset of tiles[0][0] from World coordinate (0, 0).
 */
void
Generator::generateAreas(const array& in, const sf::IntRect& area,
		const sf::Vector2f& offset) const {
	assert(area.width > 0 && area.height > 0);
	int count = countWalls(in, area);
	if (count == 0) {
		mPathfinder.insertArea(sf::IntRect(area));
	}
	else if (count == area.width * area.height) {
		return;
	}
	else {
		int halfWidth = area.width / 2.0f;
		int halfHeight = area.height / 2.0f;
		generateAreas(in, sf::IntRect(area.left,
				area.top,             halfWidth, halfHeight), offset);
		generateAreas(in, sf::IntRect(area.left + halfWidth,
				area.top,             halfWidth, halfHeight), offset);
		generateAreas(in, sf::IntRect(area.left,
				area.top + halfHeight, halfWidth, halfHeight), offset);
		generateAreas(in, sf::IntRect(area.left + halfWidth,
				area.top + halfHeight, halfWidth, halfHeight), offset);
	}
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
