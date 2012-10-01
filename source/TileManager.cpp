/*
 * TileManager.cpp
 *
 *  Created on: 08.08.2012
 *      Author: Felix
 */

#include "TileManager.h"

#include <Thor/Resources.hpp>

#include "util/Loader.h"
#include "util/ResourceManager.h"
#include "abstract/Sprite.h"

const Vector2i TileManager::TILE_SIZE = Vector2i(100, 100);

/**
 * Loads tile resources.
 *
 * @param world Box2D world to create (physical) tiles in.
 */
TileManager::TileManager(b2World& world) :
		mWorld(world) {
}

/**
 * Constructs a tile.
 *
 * @param pType Type of the tile to create.
 * @param pPosition Position of the tile in tile coordinates.
 * @param world Box2D world object.
 */
TileManager::Tile::Tile(Type type, const TilePosition& position, b2World& world) :
		Sprite(getTexture(type), PhysicalData(Vector2f(position.x * TILE_SIZE.x, position.y * TILE_SIZE.y),
				TILE_SIZE, world, CATEGORY_WORLD, (type == Type::FLOOR) ? MASK_NONE : MASK_ALL, false)),
		mType(type) {
}

/**
 * Returns a texture key for a certain tile type.
 *
 * @param type The type of tile to load a resource key for.
 * @return Resource key to the correct texture.
 */
std::shared_ptr<sf::Texture>
TileManager::Tile::getTexture(Type type) {
	sf::String filename;
	switch (type) {
	case Type::FLOOR:
		filename = "floor.png";
		break;
	case Type::WALL:
		filename = "wall.png";
		break;
	default:
		throw new aurora::Exception("Invalid tile type.");
	}
	return ResourceManager::i().acquire(Loader::i().fromFile<sf::Texture>(filename));
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
 * Fills the world with predefined tiles.
 */
void
TileManager::generate() {
	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
			setTile(TilePosition(x, y), Type::WALL);

	for (int x = 1; x < 9; x++)
		for (int y = 1; y < 9; y++)
			setTile(TilePosition(x, y), Type::FLOOR);
}

/**
 * Insert a tile at the position. Deletes an existing tile first if one is at the position.
 *
 * @param position Grid coordinate of the tile (not pixel coordinate).
 * @param type Type of tile to be inserted.
 */
void
TileManager::setTile(const TilePosition& position, Type type) {
	for (auto it = mTiles.begin(); it != mTiles.end(); it++) {
		if ((*it)->getTilePosition() == position) {
			mTiles.erase(it);
		}
	}
	mTiles.push_back(std::unique_ptr<Tile>(new Tile(type, position, mWorld)));
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
