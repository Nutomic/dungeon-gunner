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
#include "../util/String.h"
#include "../util/Vector.h"
#include "../util/Yaml.h"

class Physical;
class Yaml;

/**
 * Represents a drawable object.
 *
 * Handles drawing to world.
 */
class Sprite : public sf::Drawable, public Physical {
// Public functions.
public:
	Sprite(const Yaml& config, const PhysicalData& data, const Vector2i& size = Vector2i());
	Sprite(const std::shared_ptr<sf::Texture>& texture, const PhysicalData& data);
	virtual ~Sprite() = 0;

// Protected functions.
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Private variables.
private:
	static const String KEY_TEXTURE;

	std::shared_ptr<sf::Texture> mTexture;
	Vector2i mSize;
};

#endif /* DG_SPRITE_H_ */
