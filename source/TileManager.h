/*
 * TileManager.h
 *
 *  Created on: 08.08.2012
 *      Author: Felix
 */

#ifndef DG_TILEMANAGER_H_
#define DG_TILEMANAGER_H_

#include <map>
#include <memory>
#include <vector>

#include <Box2D/Box2D.h>

#include "util/Vector.h"
#include "abstract/Sprite.h"
#include "util/String.h"

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
	typedef Vector2i TilePosition;

// Public variables.
public:
	/// The size of a single tile (pixels).
	static const Vector2i TILE_SIZE;

// Public functions.
public:
	TileManager(b2World& world);

	void setTile(const TilePosition& position, Type type);

// Private types.
private:
	class Tile;

// Private functions.
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Private variables.
private:
	b2World& mWorld;
	std::vector<std::unique_ptr<Tile> > mTiles;
};

/**
 * Holds information about a single tile.
 */
class TileManager::Tile : public Sprite {
// Public functions.
public:
	Tile(Type type, const TilePosition& position, b2World& world);

	Type getType() const;
	TilePosition getTilePosition() const;

	static String getConfig(Type type);

// Private variables.
private:
	Type mType;
};

#endif /* DG_TILEMANAGER_H_ */
