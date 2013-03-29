/*
 * Sprite.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#include "Sprite.h"

#include <math.h>

#include <Thor/Vectors.hpp>

#include "../util/Loader.h"
#include "../util/Log.h"
#include "../util/ResourceManager.h"
#include "../util/Yaml.h"

const std::string Sprite::KEY_SIZE = "size";
const std::string Sprite::KEY_RADIUS = "radius";
const std::string Sprite::KEY_TEXTURE = "texture";

/**
 * Initializes sprite data.
 *
 * @param data Container holding construction parameters.
 * @param config Additional construction parameters
 */
Sprite::Sprite(const Data& data, const Yaml& config) :
		mCategory(data.category),
		mMask(data.mask),
		mDelete(false) {
	std::string texture = config.get<std::string>(KEY_TEXTURE, "");
	if (texture == "") {
		LOG_E("Failed to read texture from YAML file " << config.getFilename());
	}
	mTexture = ResourceManager::i().acquire(Loader::i()
			.fromFile<sf::Texture>(texture));

	float radius = config.get(KEY_RADIUS, 0.0f);
	sf::Vector2f size = config.get(KEY_SIZE, sf::Vector2f());
	if (radius != 0.0f) {
		mShape.type = Shape::Type::CIRCLE;
		mShape.shape = std::unique_ptr<sf::Shape>(new sf::CircleShape(radius));
		mShape.shape->setOrigin(radius, radius);
		mShape.shape->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(radius * 2, radius * 2)));

	}
	else if (size != sf::Vector2f()) {
		mShape.type = Shape::Type::RECTANGLE;
		mShape.shape = std::unique_ptr<sf::Shape>(new sf::RectangleShape(size));
		mShape.shape->setOrigin(size / 2.0f);
		mShape.shape->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(size)));
	}
	else {
		LOG_E("Could not read size or radius from " << config.getFilename());
	}

	mShape.shape->setTexture(&*mTexture, false);

	setPosition(data.position);
	setAngle(data.angle);
}

/**
 * Used to make this class pure virtual without any pure virtual function.
 */
Sprite::~Sprite() {
}

/**
 * Initializes container.
 */
Sprite::Data::Data(const sf::Vector2f& position, float angle,
		Category category, unsigned short mask) :
		position(position),
		angle(angle),
		category(category),
		mask(mask) {
}

/**
 * Returns the position of the sprite (center).
 */
sf::Vector2f
Sprite::getPosition() const {
	return mShape.shape->getPosition();
}

/**
 * Returns the movement speed of the sprite.
 */
sf::Vector2f
Sprite::getSpeed() const {
	return mSpeed;
}

/**
 * Returns the angle of the sprite.
 */
float
Sprite::getAngle() const {
	return mShape.shape->getRotation();
}

/**
 * Returns true if this object should be deleted.
 */
bool
Sprite::getDelete() const {
	return mDelete;
}

/**
 * Returns the Category of this object.
 */
Sprite::Category
Sprite::getCategory() const {
	return mCategory;
}

/**
 * Returns the size of the sprite as a vector (bottom left to top right),
 * does not consider rotation.
 */
sf::Vector2f
Sprite::getSize() const {
	sf::FloatRect bounds = mShape.shape->getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void
Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*mShape.shape, states);
}

/**
 * Returns true if collisions with that category are enabled through mask.
 */
bool
Sprite::collisionEnabled(Category category) const {
	return (category & mMask) != 0;
}

/**
 * Called when a collision with another Sprite occured. Override this method
 * to manage collision events.
 *
 * @param other The other Sprite in the collision.
 */
void
Sprite::onCollide(std::shared_ptr<Sprite> other) {
}

/**
 * Set to true to mark this object for deletion from the world.
 */
void
Sprite::setDelete(bool value) {
	mDelete = value;
}

/**
 * Sets movement speed and direction of the Sprite. Set either value to zero to stop movement.
 *
 * @param direction The direction the Sprite moves in, does not have to be normalized.
 * @param speed Movement speed in pixels per second.
 */
void
Sprite::setSpeed(sf::Vector2f direction, float speed) {
	if (direction != sf::Vector2f()) {
		thor::setLength(direction, speed);
	}
	mSpeed = direction;
}

/**
 * Sets the angle of the Sprite.
 */
void
Sprite::setAngle(float angle) {
	mShape.shape->setRotation(angle);
}

/**
 * Sets the position of thr Sprite.
 */
void
Sprite::setPosition(const sf::Vector2f& position) {
	mShape.shape->setPosition(position);
}

/**
 * Returns the radius of this sprite. Will fail if this is not a circle.
 *
 * @return The radius of this sprite.
 */
float
Sprite::getRadius() const {
	std::shared_ptr<sf::CircleShape> circleShape =
			std::dynamic_pointer_cast<sf::CircleShape>(mShape.shape);
	assert(circleShape);
	return circleShape->getRadius();
}
