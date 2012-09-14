/*
 * Sprite.h
 *
 *  Created on: 22.07.2012
 *      Author: Felix
 */

#ifndef DG_SPRITE_H_
#define DG_SPRITE_H_

#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include "Physical.h"
#include "../util/Vector.h"

class Physical;

/**
 * Represents a drawable object.
 *
 * Handles drawing to world.
 */
class Sprite : public sf::Drawable, public Physical {
public:
	Sprite(const sf::String& texturePath, const PhysicalData& data);
	Sprite(const std::shared_ptr<sf::Texture>& texture, const PhysicalData& data);
	virtual ~Sprite() = 0;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::shared_ptr<sf::Texture> mTexture;
	Vector2i mSize;
};

#endif /* DG_SPRITE_H_ */
