/*
 * Rectangle.cpp
 *
 *  Created on: 04.05.2013
 *      Author: Felix
 */

#include "Rectangle.h"

#include "Circle.h"
#include "../util/Yaml.h"

Rectangle::Rectangle(const sf::Vector2f& position, Category category,
		unsigned short mask, const Yaml& config,
		const sf::Vector2f& direction) :
	Sprite(position, category, mask, config.get(YAML_KEY::SIZE, sf::Vector2f()),
			config.get(YAML_KEY::TEXTURE, std::string()), direction) {
}

/**
 * Returns true if a collision between this and other occured. It does not
 * matter which object is this or other.
 */
bool
Rectangle::testCollision(std::shared_ptr<Sprite> other, int elapsed) {
	Rectangle* rect = dynamic_cast<Rectangle*>(other.get());
	Circle* circle = dynamic_cast<Circle*>(other.get());
	if (circle != nullptr)
		return CollisionModel::testCollision(*circle, *this, elapsed);
	else if (rect != nullptr)
		return CollisionModel::testCollision(*rect, *this, elapsed);
	else {
		assert(false);
		return false;
	}
}

