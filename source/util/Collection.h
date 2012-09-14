/*
 * Collection.h
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#ifndef DG_COLLECTION_H_
#define DG_COLLECTION_H_

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../abstract/Sprite.h"

class Sprite;

/**
 * A collection of sprites, which can be put into different layers.
 */
class Collection : public sf::Drawable {
// Public types.
public:
	/**
	 * Determines in what order sprites are rendered, dynamics and actors should be on top.
	 */
	enum Level {
		LEVEL_STATIC,
		LEVEL_PARTICLE,
		LEVEL_ACTOR
	};

// Public functions.
public:
	void insert(std::shared_ptr<Sprite> drawable, Level level);
	void remove(std::shared_ptr<Sprite> drawable);
	void checkDelete();

// Private functions.
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Private variables.
private:
	std::map<Level, std::vector<std::shared_ptr<Sprite> > > mDrawables;
};

#endif /* DG_COLLECTION_H_ */
