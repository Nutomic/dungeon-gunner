/*
 * Tile.h
 *
 *  Created on: 20.04.2013
 *      Author: Felix
 */

#ifndef DG_TILE_H_
#define DG_TILE_H_

#include "abstract/Rectangle.h"

class World;

/**
 * Holds information about a single tile.
 */
class Tile : public Rectangle {
public:
	enum class Type : char {
		WALL,
		FLOOR
	};
	static const Vector2i TILE_SIZE; //< Tile size in pixels.

public:
	explicit Tile(const Vector2i& tilePosition, Type type);
	Type getType() const;

	static void setTile(const Vector2i& position, Type type, World& world);
	static std::string getConfig(Type type);
	static bool isSolid(Type type);
	static Vector2f toPosition(const Vector2i& tilePosition);

private:
	Type mType;
};

#endif /* DG_TILE_H_ */
