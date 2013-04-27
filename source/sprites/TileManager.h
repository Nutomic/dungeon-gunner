/*
 * TileManager.h
 *
 *  Created on: 08.08.2012
 *      Author: Felix
 */

#ifndef DG_TILEMANAGER_H_
#define DG_TILEMANAGER_H_

#include "../abstract/Sprite.h"

class World;

class TileManager {
public:
	enum class Type {
		FLOOR,
		WALL
	};

	/**
	 * Uses the length/width of a tile as a unit.
	 */
	typedef sf::Vector2i TilePosition;

public:
	static const sf::Vector2i TILE_SIZE; //< Tile size in pixels.

public:
	explicit TileManager(World& world);

	void insertTile(const TilePosition& position, Type type);
	bool raycast(const sf::Vector2f& lineStart,
			const sf::Vector2f& lineEnd) const;

private:
	class Tile;

private:
	World& mWorld;
	std::vector<std::shared_ptr<Tile> > mTiles;
};

/**
 * Holds information about a single tile.
 */
class TileManager::Tile : public Sprite {
public:
	explicit Tile(Type type, const TilePosition& position);

	Type getType() const;
	TilePosition getTilePosition() const;

	static std::string getConfig(Type type);

private:
	Type mType;
};

#endif /* DG_TILEMANAGER_H_ */
