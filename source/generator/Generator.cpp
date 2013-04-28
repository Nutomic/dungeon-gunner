/*
 * Generator.cpp
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#include "Generator.h"

#include <assert.h>
#include <bitset>

#include "simplexnoise.h"
#include "../sprites/TileManager.h"
#include "../util/Log.h"
#include "../Pathfinder.h"

/// For usage with simplexnoise.h
uint8_t perm[512];

/**
 * Generates new random seed.
 */
Generator::Generator() {
	std::mt19937 mersenne(time(nullptr));
	std::uniform_int_distribution<int> distribution(0, 255);

	for (int i = 0; i < 512; i++) {
		perm[i] = distribution(mersenne);
	}
}

/**
 * Fill TileManager with procedurally generated tiles.
 *
 * @param tm TileManager instance to set tiles in.
 * @param area Size and position of area to generate tiles for. Must be
 * 				power of two.
 */
void
Generator::generateTiles(TileManager& tm, Pathfinder& pathfinder,
		const sf::IntRect& area) const {
	// Check if width and height are power of two.
	assert(area.width && !(area.width & (area.width - 1)));
	assert(area.height && !(area.height & (area.height - 1)));

	std::vector<std::vector<TileManager::Type> >
			noise(area.width, std::vector<TileManager::Type>(area.height));
	std::vector<std::vector<TileManager::Type> >
			filtered(area.width, std::vector<TileManager::Type>(
					area.height, TileManager::Type::FLOOR));

	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			noise[x-area.left][y-area.top] =
					(scaled_octave_noise_2d(2, 2, 0.05f, 0.5f, -0.5f, x, y) +
					scaled_octave_noise_2d(2, 2, 0.5f, 0.15f, -0.15f, x, y)
					< -0.1f)
							? TileManager::Type::WALL
							: TileManager::Type::FLOOR;
		}
	}
	for (int x = 0; x < (int) noise.size(); x++) {
		for (int y = 0; y < (int) noise[x].size(); y++) {
			filterWalls(noise, filtered, x, y, 2, 1, 0);
			filterWalls(noise, filtered, x, y, 6, 1, 2);
			filterWalls(noise, filtered, x, y, 10, 1, 4);
		}
	}
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			tm.insertTile(TileManager::TilePosition(x, y),
					filtered[x-area.left][y-area.top]);
		}
	}
	generateAreas(pathfinder, filtered, area,
			sf::Vector2f(area.left, area.top));
	pathfinder.generatePortals();
}

/**
 * Fills a rectangular area with the specified value.
 *
 * @param[in] Rectangular map.
 * @param area The area to fill.
 * @param value The value to set.
 */
void
Generator::fill(std::vector<std::vector<TileManager::Type> >& image,
		const sf::IntRect& area, TileManager::Type value) {
	for (int x = area.left;
			x < area.left + area.width && x < (int) image.size(); x++) {
		for (int y = area.top;
				y < area.top + area.height && y < (int) image[x].size(); y++) {
			image[x][y] = value;
		}
	}
}

/**
 * Returns the number of walls in the area in tiles.
 *
 * @param area The area to count in.
 * @param tiles Array of tile values.
 */
int Generator::countWalls(const sf::IntRect& area,
		std::vector<std::vector<TileManager::Type> >& tiles) {
	int count = 0;
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++)
			count += (int) (tiles[x][y] == TileManager::Type::WALL);
	}
	return count;
}

/**
 * Finds rectangles of specific size inside vector in and
 * puts them into vector out.
 *
 * @param[in] in Rectangular map of tiles.
 * @param[out] out Rectangular map of tiles.
 * @param x Position to check from (top left corner for rectangle).
 * @param y Position to check from (top left corner for rectangle).
 * @param longside Length of the longer side of the rectangle.
 * @param shortside Length of the shorter side of the rectangle.
 * @param subtract Still accepts rectangle if at least this amount of
 * 			tiles is not walls (tilecount >= longside * shortside - subtract).
 */
void
Generator::filterWalls(std::vector<std::vector<TileManager::Type> >& in,
		std::vector<std::vector<TileManager::Type> >& out,
		int x, int y, int longside, int shortside, int subtract) {
	// Skip if we would go out of range.
	if ((x + longside >= (int) in.size()) ||
			(y + longside >= (int) in[0].size()))
		return;

	// Filter in horizontal direction.
	if (countWalls(sf::IntRect(x, y, longside, shortside), in) >=
			shortside * longside - subtract)
		fill(out, sf::IntRect(x, y, longside, shortside), TileManager::Type::WALL);
	// Filter in vertical direction.
	if (countWalls(sf::IntRect(x, y, shortside, longside), in) >=
			shortside * longside - subtract)
		fill(out, sf::IntRect(x, y, shortside, longside), TileManager::Type::WALL);
}

/**
 * Inserts tile if all values within area are the same, otherwise divides area
 * into four and continues recursively.
 *
 * @param tm World to insert areas into.
 * @param tiles Array of tile values.
 * @param area The area to generate areas for.
 * @param offset Offset of tiles[0][0] from World coordinate (0, 0).
 */
void
Generator::generateAreas(Pathfinder& pathfinder,
		std::vector<std::vector<TileManager::Type> >& tiles,
		const sf::IntRect& area, const sf::Vector2f& offset) {
	assert(area.width > 0 && area.height > 0);
	int count = countWalls(sf::IntRect(area.left - offset.y, area.top - offset.x,
			area.width, area.height), tiles);
	if (count == 0) {
		pathfinder.insertArea(sf::IntRect(area));
	}
	else if (count == area.width * area.height) {
		return;
	}
	else {
		int halfWidth = area.width / 2.0f;
		int halfHeight = area.height / 2.0f;
		generateAreas(pathfinder, tiles, sf::IntRect(area.left,
				area.top,             halfWidth, halfHeight), offset);
		generateAreas(pathfinder, tiles, sf::IntRect(area.left + halfWidth,
				area.top,             halfWidth, halfHeight), offset);
		generateAreas(pathfinder, tiles, sf::IntRect(area.left,
				area.top + halfHeight, halfWidth, halfHeight), offset);
		generateAreas(pathfinder, tiles, sf::IntRect(area.left + halfWidth,
				area.top + halfHeight, halfWidth, halfHeight), offset);
	}
}
