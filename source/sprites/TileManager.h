/*
 * TileManager.h
 *
 *  Created on: 08.08.2012
 *      Author: Felix
 */

#ifndef DG_TILEMANAGER_H_
#define DG_TILEMANAGER_H_

#include <map>
#include <vector>

#include "../World.h"
#include "../abstract/Sprite.h"
#include <string>

class World;
class Sprite;

class TileManager : public sf::Drawable {
// Public types.
public:
	enum class Type {
		FLOOR,
		WALL
	};

	/**
	 * Uses the length/width of a tile as a unit.
	 */
	typedef sf::Vector2i TilePosition;

// Public variables.
public:
	/// The size of a single tile (pixels).
	static const sf::Vector2i TILE_SIZE;

// Public functions.
public:
	TileManager(World& world);

	void insertTile(const TilePosition& position, Type type);
	void removeTile(const TilePosition& position);

// Private types.
private:
	class Tile;

// Private functions.
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Private variables.
private:
	World& mWorld;
	std::vector<std::shared_ptr<Tile> > mTiles;
};

/**
 * Holds information about a single tile.
 */
class TileManager::Tile : public Sprite {
// Public functions.
public:
	Tile(Type type, const TilePosition& position);

	Type getType() const;
	TilePosition getTilePosition() const;

	static std::string getConfig(Type type);

// Private variables.
private:
	Type mType;
};

#endif /* DG_TILEMANAGER_H_ */
