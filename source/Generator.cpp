/*
 * Generator.cpp
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#include "Generator.h"

#include <bitset>

#include "simplexnoise.h"
#include "sprites/TileManager.h"

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
 * True means wall, false means floor.
 *
 * @param tm TileManager instance to set tiles in.
 * @param area Size and position of area to generate tiles for.
 */
void
Generator::generateTiles(TileManager& tm, const sf::IntRect& area) const {
	std::vector<std::vector<bool> >
			noise(area.width, std::vector<bool>(area.height));
	std::vector<std::vector<bool> >
			filtered(area.width, std::vector<bool>(area.height, false));

	for (int x = area.left; x < area.left+area.width; x++) {
		for (int y = area.top; y < area.top+area.height; y++) {
			noise[x-area.left][y-area.top] =
					(scaled_octave_noise_2d(2, 2, 0.0015f, 0.5f, -0.5f, x, y) +
					scaled_octave_noise_2d(3, 3, 0.01f, -1, 1, x, y)) < 0.05f;
		}
	}

	for (int x = 0; x < (int) noise.size(); x+=5) {
		for (int y = 0; y < (int) noise[x].size(); y+=5) {
			filterWalls(noise, filtered, x, y, 10, 5, 0);
			filterWalls(noise, filtered, x, y, 30, 5, 10);
			filterWalls(noise, filtered, x, y, 50, 5, 20);
		}
	}

	for (int x = area.left; x < area.left+area.width; x++) {
		for (int y = area.top; y < area.top+area.height; y++) {
				tm.insertTile(TileManager::TilePosition(x, y),
						(filtered[x-area.left][y-area.top])
						? TileManager::Type::WALL : TileManager::Type::FLOOR);
		}
	}
}

/**
 * Fills a rectangular area with the specified value.
 *
 * @param[in] Rectangular map.
 * @param area The area to fill.
 * @param value The value to set.
 */
void
Generator::fill(std::vector<std::vector<bool> >& image,
		const sf::IntRect& area, bool value) {
	for (int x = area.left;
			x < area.left + area.width && x < (int) image.size(); x++) {
		for (int y = area.top;
				y < area.top + area.height && y < (int) image[x].size(); y++) {
			image[x][y] = value;
		}
	}
}

/**
 * Finds rectangles of specific size in in and puts them into out.
 *
 * True means wall, false means floor.
 *
 * @param[in] in Rectangular map of walls.
 * @param[out] out Rectangular map of walls.
 * @param x Position to check from (top left corner for rectangle).
 * @param y Position to check from (top left corner for rectangle).
 * @param longside Length of the longer side of the rectangle.
 * @param shortside Length of the shorter side of the rectangle.
 * @param subtract Still accepts rectangle if at least this amount of
 * 			tiles is not walls (tilecount >= longside * shortside - subtract).
 */
void
Generator::filterWalls(std::vector<std::vector<bool> >& in,
		std::vector<std::vector<bool> >& out,
		int x, int y, int longside, int shortside, int subtract) {
	// Skip if we would go out of range.
	if ((x + longside >= (int) in.size()) ||
			(y + longside >= (int) in[0].size()))
		return;

	// Filter in horizontal direction.
	if (x % longside == 0 && y % shortside == 0) {
		int count = 0;
		for (int x2 = x; x2 < x + longside; x2++) {
			for (int y2 = y; y2 < y + shortside; y2++) {
				count += (int) in[x2][y2];
			}
		}
		if (count >= shortside * longside - subtract)
			fill(out, sf::IntRect(x, y, longside, shortside), true);
	}

	// Filter in vertical direction.
	if (x % shortside == 0 && y % longside == 0) {
		int count = 0;
		for (int x2 = x; x2 < x + shortside; x2++) {
			for (int y2 = y; y2 < y + longside; y2++)
				count += (int) in[x2][y2];
		}
		if (count >= shortside * longside - subtract)
			fill(out, sf::IntRect(x, y, shortside, longside), true);
	}
}

