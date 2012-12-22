/*
 * Sprite.cpp
 *
 *  Created on: 22.07.2012
 *      Author: Felix
 */

#include "Sprite.h"

#include "../util/Loader.h"
#include "../util/ResourceManager.h"
#include "../util/Log.h"

const std::string Sprite::KEY_TEXTURE = "texture";
const std::string Sprite::DEFAULT_TEXTURE = "";

/**
 * Loads sprite from ResourceManager, sets world position.
 *
 * @param texturePath Relative path to the texture file in the resource folder.
 */
Sprite::Sprite(const Yaml& config, const Data& data, const sf::Vector2i& size) :
		Body(data, config, size),
		mSize(sf::Vector2i(getSize())) {
	std::string texture = config.get(KEY_TEXTURE, DEFAULT_TEXTURE);
	if (texture == "") {
		LOG_E("Failed to read texture from YAML file " << config.getFilename());
	}
	mTexture = ResourceManager::i().acquire(Loader::i()
			.fromFile<sf::Texture>(texture));
}

/**
 * Used to make this class pure virtual without any pure virtual function.
 */
Sprite::~Sprite() {
}

/**
 * \copydoc sf::Drawable::draw
 */
void
Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Create a temporary shape to apply box2d body transformations to.
	sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(mSize));
	shape.setTexture(&*mTexture, true);
	shape.setOrigin(sf::Vector2f(mSize.x / 2, mSize.y / 2));
	shape.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), mSize));

	shape.setPosition(getPosition());
	shape.setRotation(getAngle());

	target.draw(shape, states);
}
