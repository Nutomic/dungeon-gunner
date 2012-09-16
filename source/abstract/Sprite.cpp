/*
 * Sprite.cpp
 *
 *  Created on: 22.07.2012
 *      Author: Felix
 */

#include "Sprite.h"

#include "../util/Loader.h"
#include "../util/String.h"
#include "../util/ResourceManager.h"

/**
 * Loads sprite from ResourceManager, sets world position.
 *
 * @param texturePath Relative path to the texture file in the resource folder.
 */
Sprite::Sprite(const sf::String& texturePath, const PhysicalData& data) :
		Physical(data),
		mTexture(ResourceManager::i().acquire(Loader::i().fromFile<sf::Texture>(texturePath))),
		mSize(data.size) {
}

/**
 * Loads sprite from ResourceManager, sets world position. Use this if the texture has already been loaded.
 *
 * @param texture Pointer to the texture to be used (must already be loaded).
 */
Sprite::Sprite(const std::shared_ptr<sf::Texture>& texture, const PhysicalData& data) :
		Physical(data),
		mTexture(texture),
		mSize(data.size) {

}
/**
 * Does nothing.
 */
Sprite::~Sprite() {
}

/**
 * \copydoc sf::Drawable::draw
 */
void
Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Create a temporary shape to apply box2d body transformations to.
	sf::RectangleShape shape = sf::RectangleShape(Vector2f(mSize));
	shape.setTexture(&*mTexture, true);
	shape.setOrigin(Vector2f(mSize.x / 2, mSize.y / 2));
	shape.setTextureRect(sf::IntRect(Vector2i(0, 0), mSize));

	shape.setPosition(getPosition());
	shape.setRotation(getAngle());

	target.draw(shape, states);
}
