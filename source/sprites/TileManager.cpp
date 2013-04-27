/*
 * TileManager.cpp
 *
 *  Created on: 08.08.2012
 *      Author: Felix
 */

#include "TileManager.h"

#include <Thor/Vectors.hpp>

#include "../util/Interval.h"
#include "../util/Loader.h"
#include "../util/Yaml.h"
#include "../World.h"

const sf::Vector2i TileManager::TILE_SIZE = sf::Vector2i(75, 75);

/**
 * Loads tile resources.
 *
 * @param world Box2D world to create (physical) tiles in.
 */
TileManager::TileManager(World& world) :
		mWorld(world) {
}

/**
 * Constructs a tile.
 *
 * @param pType Type of the tile to create.
 * @param pPosition Position of the tile in tile coordinates.
 * @param world Box2D world object.
 */
TileManager::Tile::Tile(Type type, const TilePosition& position) :
		Sprite(Data(sf::Vector2f(position.x * TILE_SIZE.x, position.y * TILE_SIZE.y),
				CATEGORY_WORLD, (type == Type::FLOOR) ? MASK_NONE : MASK_ALL),
				Yaml(getConfig(type))),
		mType(type) {
}

/**
 * Returns a texture key for a certain tile type.
 *
 * @param type The type of tile to load a resource key for.
 * @return Resource key to the correct texture.
 */
std::string
TileManager::Tile::getConfig(Type type) {
	std::string filename;
	switch (type) {
	case Type::FLOOR:
		filename = "tile_floor.yaml";
		break;
	case Type::WALL:
		filename = "tile_wall.yaml";
		break;
	default:
		throw new aurora::Exception("Invalid tile type.");
	}
	return filename;
}

/**
 * Returns the Type of this tile.
 */
TileManager::Type
TileManager::Tile::getType() const {
	return mType;
}

/**
 * Returns the position of the tile with tile width/height as a unit.
 */
TileManager::TilePosition
TileManager::Tile::getTilePosition() const {
	return TilePosition(getPosition().x / TILE_SIZE.x, getPosition().y / TILE_SIZE.y);
}

/**
 * Insert a tile at the position. Deletes an existing tile first if one is at the position.
 *
 * @param position Grid coordinate of the tile (not pixel coordinate).
 * @param type Type of tile to be inserted.
 */
void
TileManager::insertTile(const TilePosition& position, Type type) {
	#ifndef NDEBUG
	for (auto it = mTiles.begin(); it != mTiles.end(); it++) {
		if ((*it)->getTilePosition() == position)
			// Inserted multiple tiles at the same position.
			assert(false);
	}
	#endif
	std::shared_ptr<Tile> tile = std::shared_ptr<Tile>(new Tile(type, position));
	mTiles.push_back(tile);
	mWorld.insert(tile);
}

/*
 * Performs a raycast between two points to check if the path between them is
 * clear of walls.
 *
 * @param lineStart First point of the line to test.
 * @param lineEnd Second point of the line to test.
 * @return True if the ray was not blocked.
 */
bool
TileManager::raycast(const sf::Vector2f& lineStart,
		const sf::Vector2f& lineEnd) const {
	assert(lineStart != lineEnd);
	sf::Vector2f lineCenter = lineStart + 0.5f * (lineEnd - lineStart);
	for (auto it : mTiles) {
		if (it->getType() == Type::FLOOR)
			continue;
		sf::Vector2f axis = it->getPosition() - lineCenter;
		if (axis == sf::Vector2f())
			return false;

		axis = thor::unitVector(axis);
		sf::Vector2f halfsize = it->getSize() / 2.0f;
		float rectPosProjected = thor::dotProduct(axis, it->getPosition());
		float lineStartProjected = thor::dotProduct(axis, lineStart);
		float lineEndProjected = thor::dotProduct(axis, lineEnd);
		// For corner projections, those on the same line with the rect
		// center are equal by value, so we only need one on each axis
		// and take the maximum.
		float rectHalfWidthProjected = std::max(
				abs(thor::dotProduct(axis, halfsize)),
				abs(thor::dotProduct(axis,
						sf::Vector2f(halfsize.x, -halfsize.y))));
		Interval line = Interval::IntervalFromPoints(lineStartProjected, lineEndProjected);
		Interval rect = Interval::IntervalFromRadius(rectPosProjected, rectHalfWidthProjected);
		// Allow movement if sprites are moving apart.
		if (line.getOverlap(rect).getLength() > 0.0f)
			return false;
	}
	return true;
}

/**
 * \copydoc sf::Drawable::draw
 */
void
TileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto it = mTiles.begin(); it != mTiles.end(); it++) {
		target.draw((**it), states);
	}
}
