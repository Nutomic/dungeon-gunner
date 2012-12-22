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

#include "Body.h"
#include <string>
#include "../util/Yaml.h"

class Body;
class Yaml;

/**
 * Represents a drawable object.
 *
 * Handles drawing to world.
 */
class Sprite : public sf::Drawable, public Body {
// Public functions.
public:
	Sprite(const Yaml& config, const Data& data, const sf::Vector2i& size = sf::Vector2i());
	Sprite(const std::shared_ptr<sf::Texture>& texture, const Data& data);
	virtual ~Sprite() = 0;

// Protected functions.
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Private variables.
private:
	static const std::string KEY_TEXTURE;
	static const std::string DEFAULT_TEXTURE;

	std::shared_ptr<sf::Texture> mTexture;
	sf::Vector2i mSize;
};

#endif /* DG_SPRITE_H_ */
