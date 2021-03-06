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

const Vector2i Tile::TILE_SIZE = Vector2i(75, 75);

/**
 * Constructs a tile. Use this over setTile if a tile has not been generated
 * for the position yet.
 *
 * @param pType Type of the tile to create.
 */
Tile::Tile(const Vector2i& tilePosition, Type type) :
		Rectangle(toPosition(tilePosition),
				CATEGORY_WORLD,	(isSolid(type)) ? 0xffff : 0,
				Yaml(getConfig(type))),	mType(type) {
}

/**
 * Places a tile of type at position, removing an old tile of different type
 * if one exists.
 *
 * Highly inefficient as World::getNearbySprites has to be searched for every call.
 */
void
Tile::setTile(const Vector2i& position, Type type, World& world) {
	Vector2f worldPosition(thor::cwiseProduct(position, TILE_SIZE));
	auto candidates = world.getNearbySprites(worldPosition, 1.0f);
	for (auto& c : candidates) {
		std::shared_ptr<Tile> converted = std::dynamic_pointer_cast<Tile>(c);
		// Direct comparison of floats as both are from the same generation
		// on the same CPU.
		if (converted && converted->getPosition() == worldPosition &&
				converted->getType() != type) {
			world.remove(converted);
			break;
		}
	}
	world.insert(std::shared_ptr<Sprite>(new Tile(position, type)));
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
		return "";
	}
}

/**
 * Returns true if collisions with this tile type are enabled.
 */
bool
Tile::isSolid(Type type) {
	switch (type) {
	case Type::FLOOR:
		return false;
	case Type::WALL:
		return true;
	default:
		return true;
	}
}

/**
 * Converts a tile position to world/pixel position.
 */
Vector2f
Tile::toPosition(const Vector2i& tilePosition) {
	return Vector2f(thor::cwiseProduct(tilePosition, TILE_SIZE));
}

/**
 * Returns the Type of this tile.
 */
Tile::Type
Tile::getType() const {
	return mType;
}
