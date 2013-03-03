/*
 * World.h
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#ifndef DG_WORLD_H_
#define DG_WORLD_H_

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "abstract/Sprite.h"

class Sprite;
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
	void step(int elapsed);

// Private types.
private:
	class Interval {
	public:
		float start;
		float end;
		Interval(float center, float radius);
		Interval getOverlap(Interval other) const;
		float getLength();
	};

// Private functions.
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool testCollision(std::shared_ptr<Sprite> spriteA, std::shared_ptr<Sprite> spriteB, int elapsed) const;

// Private variables.
private:
	std::map<Sprite::Category, std::vector<std::shared_ptr<Sprite> > > mDrawables;
};

#endif /* DG_WORLD_H_ */
