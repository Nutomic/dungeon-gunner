/*
 * World.h
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#ifndef DG_WORLD_H_
#define DG_WORLD_H_

#include "abstract/Character.h"
#include "abstract/Sprite.h"

class Character;
class Sprite;

/**
 * A collection of sprites, which can be put into different layers.
 *
 * Uses Sprite instead of sf::Drawable to also manage deleting objects.
 * Render order is determined by Physical::Category (higher number on top).
 */
class World : public sf::Drawable {
// Public functions.
public:
	void insert(std::shared_ptr<Sprite> drawable);
	void remove(std::shared_ptr<Sprite> drawable);
	void insertCharacter(std::shared_ptr<Character> character);
	void removeCharacter(std::shared_ptr<Character> character);
	void step(int elapsed);
	void think(int elapsed);
	void generateAreas();
	std::vector<sf::Vector2f> getPath(const sf::Vector2f& start,
			const sf::Vector2f& end, float radius) const;

// Private types.
private:
	class Interval {
	public:
		float start;
		float end;
		Interval(float center, float radius);
		Interval getOverlap(Interval other) const;
		bool isInside(float point) const;
		float getLength();
	};

// Private types.
private:
	struct Area;
	/**
	 * Edges
	 *
	 * Redundant data as portals are saved twice.
	 */
	struct Portal {
		sf::Vector2f start;
		sf::Vector2f end;
		Area* area;
	};

	/**
	 * Nodes
	 */
	struct Area {
		sf::FloatRect area;
		sf::Vector2f center;
		std::vector<Portal> portals;
	};

// Private functions.
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool testCollision(std::shared_ptr<Sprite> spriteA, std::shared_ptr<Sprite> spriteB,
    		int elapsed) const;
    Area* getArea(const sf::Vector2f& point) const;
    float heuristic_cost_estimate(Area* start, Area* end) const;
    std::vector<Portal*> astarArea(Area* start, Area* end) const;

// Private variables.
private:
	std::map<Sprite::Category, std::vector<std::shared_ptr<Sprite> > > mDrawables;
	std::vector<Area> mAreas; //< This has to be a vector as objects are compared by address.
	std::vector<std::shared_ptr<Character> > mCharacters;
};

#endif /* DG_WORLD_H_ */
