/*
 * Circle.cpp
 *
 *  Created on: 04.05.2013
 *      Author: Felix
 */

#include "Circle.h"

#include "Rectangle.h"
#include "../../util/Yaml.h"

Circle::Circle(const Vector2f& position, Category category,
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
Circle::testCollision(std::shared_ptr<Sprite> other,
		Vector2f& offsetFirst, const Vector2f& offsetSecond) {
	Rectangle* rect = dynamic_cast<Rectangle*>(other.get());
	Circle* circle = dynamic_cast<Circle*>(other.get());
	if (circle != nullptr)
		return CollisionModel::testCollision(*this, *circle,
				offsetFirst, offsetSecond);
	else if (rect != nullptr)
		return CollisionModel::testCollision(*this, *rect,
				offsetFirst, offsetSecond);
	else {
		assert(false);
		return false;
	}
}

/**
 * Returns the radius of the circle used as a collision model for this object.
 */
float
Circle::getRadius() const {
	return getSize().x / 2;
}
