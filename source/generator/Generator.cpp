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
#include "../sprites/Enemy.h"
#include "../util/Log.h"

const int Generator::GENERATE_AREA_SIZE = 4;
const float Generator::GENERATE_AREA_RANGE = 4.0f;

/**
 * Generates new random seed.
 */
Generator::Generator(World& world, Pathfinder& pathfinder) :
		mWorld(world),
		mPathfinder(pathfinder) {
}

/**
 * Generates tiles near player position (maximum distance is determined by
 * GENERATE_AREA_SIZE and GENERATE_AREA_RANGE).
 */
void
Generator::generateCurrentAreaIfNeeded(const Vector2f& playerPosition) {
	std::map<Vector2i, float> open;
	std::set<Vector2i> closed;

	Vector2i start((int) floor(playerPosition.x / Tile::TILE_SIZE.x),
			(int) floor(playerPosition.y / Tile::TILE_SIZE.y));
	start /= GENERATE_AREA_SIZE;
	auto makePair = [&start](const Vector2i& point) {
		return std::make_pair(point, thor::length(Vector2f(point - start)));
	};

	open.insert(makePair(start));
	while (!open.empty()) {
		auto intComp = [](const std::pair<Vector2i, float>& left,
				const std::pair<Vector2i, float>& right) {
			return left.second < right.second;
		};
		Vector2i current =
				std::min_element(open.begin(), open.end(), intComp)->first;
		float distance = open[current];
		open.erase(current);
		closed.insert(current);
		if (!mGenerated[current.x][current.y] && distance <= GENERATE_AREA_RANGE) {
			mGenerated[current.x][current.y] = true;
			sf::IntRect area(current * GENERATE_AREA_SIZE -
					Vector2i(GENERATE_AREA_SIZE, GENERATE_AREA_SIZE) / 2,
					Vector2i(GENERATE_AREA_SIZE, GENERATE_AREA_SIZE));
			generateTiles(area);
			for (const auto& enemyPosition : getEnemySpawns(area)) {
				float distance = thor::length(enemyPosition - playerPosition);
				if (distance > Character::VISION_DISTANCE) {
					mWorld.insertCharacter(std::shared_ptr<Enemy>(new Enemy(mWorld, mPathfinder, enemyPosition)));
				}
			}
		}
		if (mGenerated[current.x][current.y] && distance <= GENERATE_AREA_RANGE) {
			if (closed.find(Vector2i(current.x + 1, current.y)) == closed.end())
				open.insert(makePair(Vector2i(current.x + 1, current.y)));
			if (closed.find(Vector2i(current.x, current.y + 1)) == closed.end())
				open.insert(makePair(Vector2i(current.x, current.y + 1)));
			if (closed.find(Vector2i(current.x - 1, current.y)) == closed.end())
				open.insert(makePair(Vector2i(current.x - 1, current.y)));
			if (closed.find(Vector2i(current.x, current.y - 1)) == closed.end())
				open.insert(makePair(Vector2i(current.x, current.y - 1)));
		}
	}
}

/**
 * Generates a minimum spanning tree on mTileNoise, starting from start with
 * a maximum total node weight of limit.
 *
 * FIXME: Some nodes are selected more than once.
 */
std::vector<Vector2i>
Generator::createMinimalSpanningTree(const Vector2i& start,
		const float limit) {
	std::vector<Vector2i> open;
	std::vector<Vector2i> selected;
	open.push_back(start);
	float totalWeight = 0.0f;

	while (totalWeight < limit) {
		Vector2i current;
		float minValue = std::numeric_limits<float>::max();
		for (auto& o : open) {
			if (mTileNoise.getNoise(o.x, o.y) + 1.0f < minValue) {
				current = o;
				minValue = mTileNoise.getNoise(o.x, o.y) + 1.0f;
			}
		}
		std::remove(open.begin(), open.end(), current);
		selected.push_back(current);
		totalWeight += minValue;

		auto insertOnlyNew = [&open, &selected](const Vector2i& v) {
			if (std::find(open.begin(), open.end(),	v) == open.end()
					&& std::find(selected.begin(), selected.end(), v) == selected.end())
				open.push_back(v);
		};
		insertOnlyNew(Vector2i(current.x + 1, current.y));
		insertOnlyNew(Vector2i(current.x, current.y + 1));
		insertOnlyNew(Vector2i(current.x - 1, current.y));
		insertOnlyNew(Vector2i(current.x, current.y - 1));
	}
	return selected;
}

/**
 * Fill world with procedurally generated tiles.
 *
 * This is done by generating random (simplex) noise, with a value mapped
 * to every integer point in area, selecting the lowest value point as start,
 * and building a minimum spanning tree from there.
 *
 * @param area Size and position of area to generate tiles for. Width and
 * 				height must each be a power of two.
 */
void
Generator::generateTiles(const sf::IntRect& area) {
	// Width and height must be a power of two.
	assert(area.width && !(area.width & (area.width - 1)));
	assert(area.height && !(area.height & (area.height - 1)));

    Vector2i start;
    float minValue = std::numeric_limits<float>::max();

    // Find lowest value for tree start.
	for (int x = area.left; x < area.left + area.width; x++)
		for (int y = area.top; y < area.top + area.height; y++) {
			if (mTileNoise.getNoise(x, y) + 1.0f < minValue) {
                start = Vector2i(x, y);
                minValue = mTileNoise.getNoise(x, y) + 1.0f;
			}
		}

	std::vector<Vector2i> selected = createMinimalSpanningTree(start, 12.0f);

    // For rooms, take minimum bounding box of spanning tree.

	int left  = start.x;
	int right = start.x;
	int down  = start.y;
	int up    = start.y;

	for (auto& s : selected) {
		if (s.x < left) left = s.x;
		if (s.x > right) right = s.x;
		if (s.y < down) down = s.y;
		if (s.y > up) up = s.y;
	}

    // Merge new map into stored map and create tile sprites.
	for (int x = area.left; x < area.left + area.width; x++)
		for (int y = area.top; y < area.top + area.height; y++) {
		    Tile::Type type = ((x >= left && x < right && y >= down && y < up)
		    		 || (mTiles[x][y] == Tile::Type::FLOOR))
                    ? Tile::Type::FLOOR
                    : Tile::Type::WALL;
			mTiles[x][y] = type;
			mWorld.insert(std::shared_ptr<Sprite>(new Tile(type, x, y)));
		}
	generateAreas(area);
	mPathfinder.generatePortals();
}

/**
 * Returns coordinates where enemies should spawn.
 *
 * @param area Area for which enemy spawns should be returned.
 */
std::vector<Vector2f>
Generator::getEnemySpawns(const sf::IntRect& area) {
	auto compare = [](const Vector2f& a, const Vector2f& b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	};
	std::set<Vector2f, decltype(compare)> ret(compare);
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			if (mCharacterNoise.getNoise(x, y) < -0.85f) {
				Vector2i tilePosition = findClosestFloor(Vector2i(x, y));
				ret.insert(Vector2f(tilePosition.x * Tile::TILE_SIZE.x,
						tilePosition.y * Tile::TILE_SIZE.y));
			}
		}
	}
	return std::vector<Vector2f>(ret.begin(), ret.end());
}

/**
 * Inserts floor tiles into path finder, using a quadtree approach to group
 * tiles where possible.
 *
 * @param area The area to generate areas for.
 */
void
Generator::generateAreas(const sf::IntRect& area) {
	assert(area.width > 0 && area.height > 0);

	int wallCount = 0;
	for (int x = area.left; x < area.left + area.width; x++)
		for (int y = area.top; y < area.top + area.height; y++)
			wallCount += (int) (mTiles[x][y] == Tile::Type::WALL);

	if (wallCount == 0)
		mPathfinder.insertArea(sf::FloatRect(area.left, area.top, area.width, area.height));
	else if (wallCount == area.width * area.height)
		return;
	else {
		int halfWidth = area.width / 2;
		int halfHeight = area.height / 2;
		generateAreas(sf::IntRect(area.left,
				area.top,             halfWidth, halfHeight));
		generateAreas(sf::IntRect(area.left + halfWidth,
				area.top,             halfWidth, halfHeight));
		generateAreas(sf::IntRect(area.left,
				area.top + halfHeight, halfWidth, halfHeight));
		generateAreas(sf::IntRect(area.left + halfWidth,
				area.top + halfHeight, halfWidth, halfHeight));
	}
}

/**
 * Returns a valid position (floor) for the player to spawn at.
 */
Vector2f
Generator::getPlayerSpawn() const {
	Vector2i spawn = findClosestFloor(Vector2i(0, 0));
	return Vector2f(spawn.x * Tile::TILE_SIZE.x,
						spawn.y * Tile::TILE_SIZE.y);
}

/**
 * Finds the point array index closest to position which has a floor tile.
 *
 * @warn Will fail if no floor tile has been generated yet.
 * @position Point to start search for a floor tile from.
 */
Vector2i
Generator::findClosestFloor(const Vector2i& position) const {
	std::map<Vector2i, float> open;
	std::set<Vector2i> closed;
	Vector2i start = position;
	auto makePair = [&start](const Vector2i& point) {
		return std::make_pair(point, thor::length(Vector2f(point - start)));
	};

	open.insert(makePair(start));
	while (!open.empty()) {
		auto intComp = [](const std::pair<Vector2i, float>& left,
				const std::pair<Vector2i, float>& right) {
			return left.second < right.second;
		};
		Vector2i current = std::min_element(open.begin(), open.end(), intComp)->first;
		open.erase(current);
		closed.insert(current);
		if (mTiles.count(current.x) != 0 &&
				mTiles.at(current.x).count(current.y) != 0 &&
				mTiles.at(current.x).at(current.y) == Tile::Type::FLOOR) {
			return current;
		}
		else {
			if (closed.find(Vector2i(current.x + 1, current.y)) == closed.end())
				open.insert(makePair(Vector2i(current.x + 1, current.y)));
			if (closed.find(Vector2i(current.x, current.y + 1)) == closed.end())
				open.insert(makePair(Vector2i(current.x, current.y + 1)));
			if (closed.find(Vector2i(current.x - 1, current.y)) == closed.end())
				open.insert(makePair(Vector2i(current.x - 1, current.y)));
			if (closed.find(Vector2i(current.x, current.y - 1)) == closed.end())
				open.insert(makePair(Vector2i(current.x, current.y - 1)));
		}
	}
	// No floor tile found in the entire world.
	assert(false);
	return Vector2i();
}
