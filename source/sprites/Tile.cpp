/*
 * Tile.cpp
 *
 *  Created on: 20.04.2013
 *      Author: Felix
 */

#include "Tile.h"

#include <Thor/Vectors.hpp>

#include "../util/Interval.h"
#include "../util/Loader.h"
#include "../util/Yaml.h"
#include "../World.h"

const sf::Vector2i Tile::TILE_SIZE = sf::Vector2i(75, 75);

/**
 * Constructs a tile.
 *
 * @param pType Type of the tile to create.
 * @param pPosition Position of the tile in tile coordinates.
 * @param world Box2D world object.
 */
Tile::Tile(Type type, int x, int y) :
		Rectangle(sf::Vector2f(x * TILE_SIZE.x, y * TILE_SIZE.y),
				CATEGORY_WORLD, (isSolid(type)) ? 0xffff : 0,
				Yaml(getConfig(type))),
		mType(type) {
}

/**
 * Returns the YAML config file name for the tile type.
 */
std::string
Tile::getConfig(Type type) {
	switch (type) {
	case Type::FLOOR:
		return "tile_floor.yaml";
	case Type::WALL:
		return "tile_wall.yaml";
	default:
		assert(false);
		return "";
	}
}
bool
Tile::isSolid(Type type) {
	switch (type) {
	case Type::FLOOR:
		return false;
	case Type::WALL:
		// falltrough
	default:
		return true;
	}
}

/**
 * Returns the Type of this tile.
 */
Tile::Type
Tile::getType() const {
	return mType;
}
