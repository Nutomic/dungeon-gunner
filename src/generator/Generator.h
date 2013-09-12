/*
 * Generator.h
 *
 *  Created on: 07.04.2013
 *      Author: Felix
 */

#ifndef DG_GENERATOR_H_
#define DG_GENERATOR_H_

#include <SFML/Graphics.hpp>

#include <LTBL/Light/LightSystem.h>

#include "../sprites/abstract/Character.h"
#include "../sprites/Tile.h"
#include "SimplexNoise.h"
#include "../util/Vector.h"

class World;
class Pathfinder;
class Yaml;

/**
 * Procedurally generates tiles, chooses player and enemy spawn positions.
 */
class Generator : public sf::Drawable {
public:
	explicit Generator(World& world, Pathfinder& pathfinder,
			ltbl::LightSystem& lightSystem, const Yaml& config);
	void generateCurrentAreaIfNeeded(const Vector2f& position,
			const Character::EquippedItems& playerItems);
	Vector2f getPlayerSpawn() const;
	std::vector<Vector2f> getEnemySpawns(const sf::IntRect& area);

private:
	typedef std::map<int, std::map<int, Tile::Type> > array;

private:
	void generateAreas(const sf::IntRect& area);
	void generateTiles(const sf::IntRect& area);
	Vector2i findClosestFloor(const Vector2i& start) const;
	std::vector<Vector2i> createMinimalSpanningTree(
			const Vector2i& start, const float limit);
	void connectRooms(const Vector2i& start);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	const int mAreaSize;
	const float mMaxRange;
	const float mRoomSizeValue;
	const float mRoomConnectionValue;
	const float mEnemyGenerationChance;

	World& mWorld;
	Pathfinder& mPathfinder;
	ltbl::LightSystem& mLightSystem;
	/// Contains values of all tiles that have yet been generated.
	array mTiles;
	/// Stores where tiles have already been generated.
	std::map<int, std::map<int, bool> > mGenerated;
	/// Perlin noise used for tile generation.
	SimplexNoise mTileNoise;
	/// Perlin noise used for character placement.
	SimplexNoise mCharacterNoise;
	std::vector<ltbl::ConvexHull*> mHulls;
	/// Used only for debug drawing.
	std::vector<std::vector<Vector2i> > mPaths;
};

#endif /* DG_GENERATOR_H_ */
