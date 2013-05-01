/*
 * Tile.h
 *
 *  Created on: 20.04.2013
 *      Author: Felix
 */

#ifndef DG_TILE_H_
#define DG_TILE_H_

#include "../abstract/Sprite.h"

/**
 * Holds information about a single tile.
 */
class Tile : public Sprite {
public:
	enum class Type : char {
		FLOOR,
		WALL
	};

public:
	explicit Tile(Type type, int x, int y);

	Type getType() const;

public:
	static const sf::Vector2i TILE_SIZE; //< Tile size in pixels.

public:
	static std::string getConfig(Type type);
	static bool isSolid(Type type);


private:
	Type mType;
};

#endif /* DG_TILE_H_ */