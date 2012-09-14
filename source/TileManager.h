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

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

#include "util/Vector.h"
#include "abstract/Sprite.h"

class Sprite;

class TileManager : public sf::Drawable {
// Public constants.
public:
	/// The size of a single tile (pixels).
	static const Vector2i TILE_SIZE;

// Public functions.
public:
	TileManager(b2World& world);

	void generate();

// Private types.
private:
	enum Type {
		TYPE_FLOOR,
		TYPE_WALL
	};

	/**
	 * Uses the length/width of a tile as a unit.
	 */
	typedef Vector2i TilePosition;

	class Tile;

// Private functions.
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setTile(const TilePosition& position, Type type);

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

	static std::shared_ptr<sf::Texture> getTexture(Type type);

// Private variables.
private:
	Type mType;
};



#endif /* DG_TILEMANAGER_H_ */
