/*
 * Rectangle.cpp
 *
 *  Created on: 04.05.2013
 *      Author: Felix
 */

#include "Rectangle.h"

#include "Circle.h"
#include "../../util/Yaml.h"

Rectangle::Rectangle(const Vector2f& position, Category category,
		unsigned short mask, const Yaml& config,
		const Vector2f& direction) :
	Sprite(position, category, mask, config.get("size", Vector2f()),
			config.get("texture", std::string()), direction) {
}

/**
 * Returns true if a collision between this and other occured. It does not
 * matter which object is this or other.
 */
bool
Rectangle::testCollision(std::shared_ptr<Sprite> other,
		Vector2f& offsetFirst, const Vector2f& offsetSecond) {
	Rectangle* rect = dynamic_cast<Rectangle*>(other.get());
	Circle* circle = dynamic_cast<Circle*>(other.get());
	if (circle != nullptr)
		return CollisionModel::testCollision(*circle, *this,
				offsetFirst, offsetSecond);
	else if (rect != nullptr)
		return CollisionModel::testCollision(*rect, *this,
				offsetFirst, offsetSecond);
	else {
		assert(false);
		return false;
	}
}

