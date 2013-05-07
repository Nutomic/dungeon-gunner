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
 * Tests for collision between rectangle and circle.
 *
 * @return True if a collision occured.
 */
bool
CollisionModel::testCollision(const Circle& circle, const Rectangle& rect,
		int elapsed) {
	sf::Vector2f halfsize = rect.getSize() / 2.0f;
	sf::Vector2f circlePos = circle.getPosition();
	sf::Vector2f rectPos = rect.getPosition();
	// Only circle movement as rectangles don't move.
	sf::Vector2f circleMovement = circle.getSpeed() * (elapsed / 1000.0f);

	// We assume that rectangles are always axis aligned.
	float overlapNoMovementX = Interval::IntervalFromRadius(circlePos.x, circle.getRadius())
							.getOverlap(Interval::IntervalFromRadius(rectPos.x, halfsize.x)).getLength();
	float overlapMovementX = Interval::IntervalFromRadius(circlePos.x + circleMovement.x, circle.getRadius())
							.getOverlap(Interval::IntervalFromRadius(rectPos.x, halfsize.x)).getLength();
	float overlapNoMovementY = Interval::IntervalFromRadius(circlePos.y, circle.getRadius())
							.getOverlap(Interval::IntervalFromRadius(rectPos.y, halfsize.y)).getLength();
	float overlapMovementY = Interval::IntervalFromRadius(circlePos.y + circleMovement.y, circle.getRadius())
							.getOverlap(Interval::IntervalFromRadius(rectPos.y, halfsize.y)).getLength();

	bool xyCollisionResult = (((overlapNoMovementX < overlapMovementX) &&
			(overlapNoMovementY > 0)) ||
			((overlapNoMovementY < overlapMovementY) && (overlapNoMovementX > 0)));
	// If circle center is overlapping rectangle on x or y axis, we can take xyCollisionResult.
	if (Interval::IntervalFromRadius(rectPos.x, halfsize.x).isInside(circlePos.x) ||
			Interval::IntervalFromRadius(rectPos.y, halfsize.y).isInside(circlePos.y))
		return xyCollisionResult;
	// Test if the circle is colliding with a corner of the rectangle.
	else if (xyCollisionResult) {
		// This is the same as circle-circle collision.
		sf::Vector2f axis = circlePos - rectPos;
		// If both objects are at the exact same position, allow any
		// movement for unstucking.
		if (axis == sf::Vector2f())
			return true;
		axis = thor::unitVector(axis);

		float circlePosProjected = thor::dotProduct(axis, circlePos);
		float movementProjected = thor::dotProduct(axis, circleMovement);
		float rectPosProjected = thor::dotProduct(axis, rectPos);
		// For corner projections, those on the same line with the rect
		// center are equal by value, so we only need one on each axis
		// and take the maximum.
		float rectHalfWidthProjected = std::max(
				abs(thor::dotProduct(axis, halfsize)),
				abs(thor::dotProduct(axis,
						sf::Vector2f(halfsize.x, -halfsize.y))));

		// Allow movement if sprites are moving apart.
		return Interval::IntervalFromRadius(circlePosProjected, circle.getRadius())
						.getOverlap(Interval::IntervalFromRadius(rectPosProjected,
								rectHalfWidthProjected))
						.getLength() <
				Interval::IntervalFromRadius(circlePosProjected + movementProjected, circle.getRadius())
						.getOverlap(Interval::IntervalFromRadius(rectPosProjected,
								rectHalfWidthProjected))
						.getLength();
	}
	// If there is no collision on x and y axis, there can't be one at all.
	else {
		return false;
	}

}

/**
 * Tests for collision between two circles.
 *
 * @return True if a collision occured.
 */
bool
CollisionModel::testCollision(const Circle& first, const Circle& second,
		int elapsed) {
	sf::Vector2f axis = first.getPosition() - second.getPosition();
	// If both objects are at the exact same position, allow any movement for unstucking.
	if (axis == sf::Vector2f())
		return true;
	axis = thor::unitVector(axis);
	float centerA = thor::dotProduct(axis, first.getPosition());
	float radiusA = first.getRadius();
	float movementA = thor::dotProduct(axis, first.getSpeed() * (elapsed / 1000.0f));
	float centerB = thor::dotProduct(axis, second.getPosition());
	float radiusB = second.getRadius();
	float movementB = thor::dotProduct(axis, second.getSpeed() * (elapsed / 1000.0f));

	// Allow movement if sprites are moving apart.
	return Interval::IntervalFromRadius(centerA, radiusA).getOverlap(
			Interval::IntervalFromRadius(centerB, radiusB)).getLength() <
			Interval::IntervalFromRadius(centerA + movementA, radiusA).getOverlap(
					Interval::IntervalFromRadius(centerB + movementB, radiusB)).getLength();

}

/**
 * Tests for collision between two rectangles. Not implemented as these can't
 * occur (rectangles can't move).
 *
 * @return True if a collision occured.
 */
bool
CollisionModel::testCollision(const Rectangle& first, const Rectangle& second,
		int elapsed) {
	return false;
}
