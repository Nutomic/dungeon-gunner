/*
 * TileManager.cpp
 *
 *  Created on: 08.08.2012
 *      Author: Felix
 */

#include "TileManager.h"

#include "../util/Loader.h"
#include "../util/Yaml.h"
#include "../World.h"

const sf::Vector2i TileManager::TILE_SIZE = sf::Vector2i(100, 100);

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
				0, CATEGORY_WORLD, (type == Type::FLOOR) ? MASK_NONE : MASK_ALL),
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
		if ((*it)->getTilePosition() == position) {
			// Inserted multiple tiles at the same position.
			assert(false);
		}
	}
	#endif
	std::shared_ptr<Tile> tile = std::shared_ptr<Tile>(new Tile(type, position));
	mTiles.push_back(tile);
	mWorld.insert(tile);
}


void
TileManager::removeTile(const TilePosition& position) {
	for (auto it = mTiles.begin(); it != mTiles.end(); it++) {
		if ((*it)->getTilePosition() == position) {
			mWorld.remove(*it);
			mTiles.erase(it);
		}
	}

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
