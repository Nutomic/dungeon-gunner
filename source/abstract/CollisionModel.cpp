/*
 * CollisionModel.cpp
 *
 *  Created on: 07.05.2013
 *      Author: Felix
 */

#include "CollisionModel.h"

#include <SFML/System.hpp>

#include <Thor/Vectors.hpp>

#include "Circle.h"
#include "Rectangle.h"
#include "../util/Interval.h"

CollisionModel::~CollisionModel() {
}

/**
 * Tests for collision between a circle and a rectangle. Offset is the maximum
 * value between zero and the original value of previous, for which the
 * objects do not collide. Rectangles are assumed to be axis aligned.
 *
 * @param [in,out] offset The movement offset of the circle.
 * @param offsetSecond Movement offset of the rectangle.
 * @return True if a collision occured.
 */
bool
CollisionModel::testCollision(const Circle& circle, const Rectangle& rect,
		sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond) {
	sf::Vector2f halfSize = rect.getSize() / 2.0f;
	sf::Vector2f rectNewPos = rect.getPosition() + offsetSecond;
	sf::Vector2f circleRotatedPos = circle.getPosition() + offsetFirst - rectNewPos;
	circleRotatedPos = thor::rotatedVector(circleRotatedPos, -rect.mShape.getRotation());
	circleRotatedPos += rectNewPos;

	// If circle center is inside rect on x plane, we just take y direction result.
	if (Interval::IntervalFromRadius(rectNewPos.x, halfSize.x)
			.isInside(circleRotatedPos.x)) {
		float overlapY =
				Interval::IntervalFromRadius(circleRotatedPos.y, circle.getRadius())
				.getOverlap(Interval::IntervalFromRadius(rectNewPos.y, halfSize.y))
						.getLength();
		offsetFirst += ((circleRotatedPos.y > rectNewPos.y) ? 1.0f : - 1.0f) *
				thor::rotatedVector(sf::Vector2f(0, overlapY), rect.mShape.getRotation());
		return overlapY > 0;
	}
	// Same here (just switched x/y).
	else if (Interval::IntervalFromRadius(rectNewPos.y, halfSize.y)
			.isInside(circleRotatedPos.y)) {
		float overlapX =
				Interval::IntervalFromRadius(circleRotatedPos.x, circle.getRadius())
				.getOverlap(Interval::IntervalFromRadius(rectNewPos.x, halfSize.x))
						.getLength();
		offsetFirst += ((circleRotatedPos.x > rectNewPos.x) ? 1.0f : - 1.0f) *
				thor::rotatedVector(sf::Vector2f(overlapX, 0), rect.mShape.getRotation());
		return overlapX > 0;
	}
	// Test if the circle is colliding with a corner of the rectangle, using
	// the same method as circle-circle collision (distance to corner instead
	// of radius.
	else {
		sf::Vector2f axis(thor::unitVector(rectNewPos - circleRotatedPos));

		// Use correct vector for corner projections (positive/negative
		// direction does not matter).
		float rectHalfSizeProjected;
		if ((circleRotatedPos.x > rectNewPos.x && circleRotatedPos.y > rectNewPos.y) ||
				(circleRotatedPos.x < rectNewPos.x && circleRotatedPos.y < rectNewPos.y))
			rectHalfSizeProjected = thor::dotProduct(axis, halfSize);
		else
			rectHalfSizeProjected = thor::dotProduct(axis,
					sf::Vector2f(halfSize.x, -halfSize.y));

		Interval projectedCircle = Interval::IntervalFromRadius(
				thor::dotProduct(axis, circleRotatedPos),
				circle.getRadius());
		Interval projectedRect = Interval::IntervalFromRadius(
				thor::dotProduct(axis, rectNewPos),
				rectHalfSizeProjected);
		// using -5: works perfectly going between corner/side
		// without -5 works perfectly on corner, but skips when going in between tiles
		float overlap = projectedCircle.getOverlap(projectedRect).getLength() - 5;
		if (overlap > 0)
			offsetFirst -= thor::rotatedVector(overlap * axis, rect.mShape.getRotation());
		return overlap > 0;
	}
}

/**
 * Tests for collision between two circles. Offset is the maximum value between
 * zero and the original value of previous, for which the objects do
 * not collide.
 *
 * @param [in,out] offset The movement offset of the first circle.
 * @param offsetSecond Movement offset of the second circle.
 * @return True if a collision occured.
 */
bool
CollisionModel::testCollision(const Circle& first, const Circle& second,
		sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond) {
	sf::Vector2f axis(thor::unitVector(second.getPosition() + offsetFirst -
			(first.getPosition() + offsetSecond)));
	Interval projectedFirst = Interval::IntervalFromRadius(
			thor::dotProduct(axis, first.getPosition() + offsetFirst),
			first.getRadius());
	Interval projectedSecond = Interval::IntervalFromRadius(
			thor::dotProduct(axis, second.getPosition() + offsetSecond),
			second.getRadius());

	float overlap = projectedFirst.getOverlap(projectedSecond).getLength();
	if (overlap > 0)
		offsetFirst -= overlap * axis;
	return overlap > 0;

}

/**
 * Tests for collision between two rectangles. Always returns false as
 * these can't occur (rectangles can't move).
 *
 * @return True if a collision occured.
 */
bool
CollisionModel::testCollision(const Rectangle& first, const Rectangle& second,
		sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond) {
	return false;
}
