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
#include "../util/Yaml.h"

/**
 * Generates new random seed.
 */
Generator::Generator(World& world, Pathfinder& pathfinder,
		ltbl::LightSystem& lightSystem, const Yaml& config) :
		mAreaSize(config.get("generate_area_size", 1)),
		mMaxRange((config.get("generate_area_range", 1.0f) / mAreaSize) / Tile::TILE_SIZE.x),
		mRoomSizeValue(config.get("room_size_value", 1.0f)),
		mRoomConnectionValue(config.get("room_connection_value", 1.0f)),
		mEnemyGenerationChance(config.get("enemy_generation_chance", 0.0f) * 2 - 1),
		mWorld(world),
		mPathfinder(pathfinder),
		mLightSystem(lightSystem) {
}

/**
 * Generates tiles near player position (maximum distance is determined by
 * GENERATE_AREA_SIZE and GENERATE_AREA_RANGE).
 */
void
Generator::generateCurrentAreaIfNeeded(const Vector2f& playerPosition,
		const Character::EquippedItems& playerItems) {
	std::map<Vector2i, float> open;
	std::set<Vector2i> closed;

	Vector2i start((int) floor(playerPosition.x / Tile::TILE_SIZE.x),
			(int) floor(playerPosition.y / Tile::TILE_SIZE.y));
	start /= mAreaSize;
	auto makePair = [&start](const Vector2i& point) {
		return std::make_pair(point, thor::length(Vector2f(point - start)));
	};

	open.insert(makePair(start));
	while (!open.empty()) {
		Vector2i current = std::min_element(open.begin(), open.end())->first;
		float distance = open[current];
		open.erase(current);
		closed.insert(current);
		if (!mGenerated[current.x][current.y] && distance <= mMaxRange) {
			mGenerated[current.x][current.y] = true;
			sf::IntRect area(current * mAreaSize -
					Vector2i(mAreaSize, mAreaSize) / 2,
					Vector2i(mAreaSize, mAreaSize));
			generateTiles(area);
			for (const auto& spawn : getEnemySpawns(area)) {
				float distance = thor::length(spawn - playerPosition);
				if (distance > Character::VISION_DISTANCE)
					mWorld.insertCharacter(std::shared_ptr<Enemy>(new Enemy(
							mWorld, mPathfinder, spawn, playerItems)));
			}
		}
		if (mGenerated[current.x][current.y] && distance <= mMaxRange) {
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
			if (mTileNoise.getNoise(o) + 1.0f < minValue) {
				current = o;
				minValue = mTileNoise.getNoise(o) + 1.0f;
			}
		}
		std::remove(open.begin(), open.end(), current);
		selected.push_back(current);
		totalWeight += minValue;

		auto insertNew = [&open, &selected](const Vector2i& v) {
			if (std::find(open.begin(), open.end(),	v) == open.end()
					&& std::find(selected.begin(), selected.end(), v) == selected.end())
				open.push_back(v);
		};
		insertNew(Vector2i(current.x + 1, current.y));
		insertNew(Vector2i(current.x, current.y + 1));
		insertNew(Vector2i(current.x - 1, current.y));
		insertNew(Vector2i(current.x, current.y - 1));
	}
	return selected;
}

/**
 * Generates paths that connect different rooms.
 *
 * Using basically Dijkstra on infinite graph/A* without destination node.
 *
 * @param start Tile to start path generation from (must be floor).
 */
void
Generator::connectRooms(const Vector2i& start) {
	std::set<Vector2i> open;
	std::set<Vector2i> closed;
	std::map<Vector2i, Vector2i> previous;
	std::set<Vector2i> destinations;
	std::map<Vector2i, float> distance;
	// Compares vectors using distance values.
	auto comp = [&distance](const Vector2i& lhs, const Vector2i& rhs) {
		return distance[lhs] < distance[rhs];
	};
	auto process = [&open, &closed, &previous, &distance, this](const Vector2i& point, const Vector2i& current) {
		// Update previous nodes if shorter path is found.
		if (distance.count(point) == 0) {
			distance[point] = distance[current] + mTileNoise.getNoise(point) + 1;
			previous[point] = current;
		}

		// Insert into open
		if (closed.count(point) == 0)
			open.insert(point);
	};

	open.insert(start);
	distance[start] = 0;
	while (!open.empty()) {
		// Select node with lowest distance that is in open
		Vector2i current = *std::min_element(open.begin(), open.end(), comp);
		open.erase(current);
		closed.insert(current);
		// Take all floors right after wall tiles.
		if (mTiles[previous[current].x][previous[current].y] == Tile::Type::WALL &&
				mTiles[current.x][current.y] == Tile::Type::FLOOR) {
			destinations.insert(current);
			break;
		}
		if (distance.at(current) < mRoomConnectionValue) {
			process(Vector2i(current.x + 1, current.y), current);
			process(Vector2i(current.x,     current.y + 1), current);
			process(Vector2i(current.x - 1, current.y), current);
			process(Vector2i(current.x,     current.y - 1), current);
		}
	}

	float totalValue = 0.0f;
	while (totalValue < mRoomConnectionValue && !destinations.empty()) {
		std::vector<Vector2i> path;
		float pathValue = 0;
		Vector2i current = *destinations.begin();
		destinations.erase(destinations.begin());
		while (current != start) {
			path.push_back(previous[current]);
			pathValue += mTileNoise.getNoise(current);
			current = previous[current];
		};
		path.push_back(start);
		mPaths.push_back(path);
		for (const auto& p : path) {
			mTiles[p.x][p.y] = Tile::Type::FLOOR;
			Tile::setTile(p, Tile::Type::FLOOR, mWorld);
			for (auto it = mHulls.begin(); it != mHulls.end(); it++)
				if ((*it)->GetWorldCenter() == Tile::toPosition(Vector2i(p.x, p.y)).toVec2f()) {
					mLightSystem.RemoveConvexHull(*it);
					mHulls.erase(it);
					break;
				}
		}
	}
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

	std::vector<Vector2i> selected = createMinimalSpanningTree(start, mRoomSizeValue);

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
	for (int x = left; x < right; x++)
		for (int y = down; y < up; y++)
			// Make sure tiles are not set twice (which would get values in
			// mTiles out of sync with actual world).
			if (mTiles[x].count(y) == 0)
				mTiles[x][y] = Tile::Type::FLOOR;

	connectRooms(start);
	for (int x = area.left; x < area.left + area.width; x++)
		for (int y = area.top; y < area.top + area.height; y++) {
			mWorld.insert(std::shared_ptr<Sprite>(
					new Tile(Vector2i(x, y), mTiles[x][y])));
			if (mTiles[x][y] == Tile::Type::WALL) {
				ltbl::ConvexHull* tileHull = new ltbl::ConvexHull();
				tileHull->m_vertices.push_back(Vec2f(-37.5f,  37.5f));
				tileHull->m_vertices.push_back(Vec2f(-37.5f, -37.5f));
				tileHull->m_vertices.push_back(Vec2f( 37.5f, -37.5f));
				tileHull->m_vertices.push_back(Vec2f( 37.5f,  37.5f));
				tileHull->m_renderLightOverHull = false;
				tileHull->CalculateNormals();
				tileHull->CalculateAABB();
				tileHull->SetWorldCenter(Tile::toPosition(Vector2i(x, y)).toVec2f());
				mLightSystem.AddConvexHull(tileHull);
				mHulls.push_back(tileHull);
			}
		}

	generateAreas(area);
	mPathfinder.generatePortals();
}

/**
 * Returns coordinates where enemies should spawn.
 *
 * @param area Area for which enemy spawns should be returned.
 * @return Pairs of spawn points together with seeds.
 */
std::vector<Vector2f>
Generator::getEnemySpawns(const sf::IntRect& area) {
	std::vector<Vector2f> spawns;
	for (int x = area.left; x < area.left + area.width; x++) {
		for (int y = area.top; y < area.top + area.height; y++) {
			float noise = mCharacterNoise.getNoise(x, y);
			if (noise <= mEnemyGenerationChance) {
				Vector2i tilePosition = findClosestFloor(Vector2i(x, y));
				spawns.push_back(Vector2f(tilePosition.x * Tile::TILE_SIZE.x,
						tilePosition.y * Tile::TILE_SIZE.y));
			}
		}
	}
	return spawns;
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
	Vector2i spawn = findClosestFloor(Vector2i());
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
Generator::findClosestFloor(const Vector2i& start) const {
	std::map<Vector2i, float> open;
	std::set<Vector2i> closed;
	auto insertNew = [&open, &closed, &start](const Vector2i& point) {
		if (closed.find(point) == closed.end())
			open.insert(std::make_pair(point, thor::length(Vector2f(point - start))));
	};

	insertNew(start);
	while (!open.empty()) {
		Vector2i current = std::min_element(open.begin(), open.end())->first;
		open.erase(current);
		closed.insert(current);
		if (mTiles.count(current.x) != 0 && mTiles.at(current.x).count(current.y) != 0 &&
				mTiles.at(current.x).at(current.y) == Tile::Type::FLOOR)
			return current;
		else {
			insertNew(Vector2i(current.x + 1, current.y));
			insertNew(Vector2i(current.x,     current.y + 1));
			insertNew(Vector2i(current.x - 1, current.y));
			insertNew(Vector2i(current.x,     current.y - 1));
		}
	}
	// No floor tile found in the entire world.
	assert(false);
	return Vector2i();
}

/**
 * Debug only: Draws paths generated by connectRooms.
 *
 * mPaths is only required for this function.
 */
void
Generator::draw(sf::RenderTarget& target, sf::RenderStates states) const {
#ifndef RELEASE
	for (auto& p : mPaths) {
		for (auto&q : p) {
			sf::RectangleShape rect(Vector2f(Tile::TILE_SIZE));
			rect.setPosition(Vector2f(q.x * Tile::TILE_SIZE.x, q.y * Tile::TILE_SIZE.y) - Vector2f(Tile::TILE_SIZE / 2));
			rect.setFillColor(sf::Color(150, 127, 0, 96));
			target.draw(rect);
		}
	}
#endif /* RELEASE */
}
