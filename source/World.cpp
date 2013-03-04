/*
 * World.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "World.h"

#include <algorithm>
#include <assert.h>

#include <Thor/Vectors.hpp>

#include "util/Log.h"

/**
 * Insert a drawable into the group. Drawables should only be handled with shared_ptr.
 * An object can't be inserted more than once at the same level.
 */
void
World::insert(std::shared_ptr<Sprite> drawable) {
#ifndef NDEBUG
	Sprite::Category cat = drawable->getCategory();
	auto item = std::find(mDrawables[cat].begin(), mDrawables[cat].end(), drawable);
	assert(item == mDrawables[cat].end());
#endif
		mDrawables[drawable->getCategory()].push_back(drawable);
}

/**
 * Removes a drawable from the group.
 */
void
World::remove(std::shared_ptr<Sprite> drawable) {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		auto item = std::find(v->second.begin(), v->second.end(), drawable);
		if (item != v->second.end()) {
			   v->second.erase(item);
		}
	}
}

/**
 * Checks for collisions and applies movement, also removes sprites if
 * Sprite::getDelete returns true.
 *
 * This method can be improved by only testing each pair of sprites once,
 * and using the result for both. Applying movement should be done in
 * testCollision, always applying the part that causes no collision.
 */
void
World::step(int elapsed) {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto it = v->second.begin(); it != v->second.end(); ) {
			auto spriteA = *it;
			if (spriteA->getDelete()) {
				remove(spriteA);
			}
			else {
				sf::Vector2f speed = spriteA->getSpeed();
				speed *= elapsed / 1000.0f;
				bool overlap = false;
				for (auto w = mDrawables.begin(); w != mDrawables.end(); w++) {
					for (auto spriteB : w->second) {
						if (spriteA == spriteB) {
							continue;
						}
						// Ignore anything that is filtered by masks.
						if (!spriteA->collisionEnabled(spriteB->getCategory()) ||
								!spriteB->collisionEnabled(spriteA->getCategory())) {
							continue;
						}
						if (testCollision(spriteA, spriteB, elapsed)) {
							spriteA->onCollide(spriteB);
							overlap = true;
						}
					}
				}
				if (!overlap) {
					spriteA->setPosition(spriteA->getPosition() + speed);
				}
				it++;
			}
		}
	}
}

/**
 * Tests for collisions using Seperating Axis Theorem (SAT).
 *
 * http://www.metanetsoftware.com/technique/tutorialA.html
 *
 * @param spriteA, spriteB Pair of sprites which to test for collision/overlapping.
 * @param elapsed Time elapsed in this step.
 * @return True if both sprites will be overlapping after their current movement.
 */
bool
World::testCollision(std::shared_ptr<Sprite> spriteA,
		std::shared_ptr<Sprite> spriteB, int elapsed) const {
	// circle-circle collision
	if ((spriteA->mShape.type == Sprite::Shape::Type::CIRCLE) &&
			(spriteB->mShape.type == Sprite::Shape::Type::CIRCLE)) {

		sf::Vector2f axis = spriteA->getPosition() - spriteB->getPosition();
		// If both objects are at the exact same position, allow any movement for unstucking.
		if (axis == sf::Vector2f()) {
			return true;
		}
		axis = thor::unitVector(axis);
		float centerA = thor::dotProduct(axis, spriteA->getPosition());
		float radiusA = std::static_pointer_cast<sf::CircleShape>(
				spriteA->mShape.shape)->getRadius();
		float movementA = thor::dotProduct(axis, spriteA->getSpeed() * (elapsed / 1000.0f));
		float centerB = thor::dotProduct(axis, spriteB->getPosition());
		float radiusB = std::static_pointer_cast<sf::CircleShape>(
				spriteB->mShape.shape)->getRadius();
		float movementB = thor::dotProduct(axis, spriteB->getSpeed() * (elapsed / 1000.0f));

		// Allow movement if sprites are moving apart.
		return Interval(centerA, radiusA).getOverlap(Interval(centerB, radiusB)).getLength() <
				Interval(centerA + movementA, radiusA).getOverlap(
						Interval(centerB + movementB, radiusB)).getLength();
	}
	// circle-rect collision
	if (((spriteA->mShape.type == Sprite::Shape::Type::CIRCLE) &&
			(spriteB->mShape.type == Sprite::Shape::Type::RECTANGLE)) ||
			((spriteA->mShape.type == Sprite::Shape::Type::RECTANGLE) &&
			(spriteB->mShape.type == Sprite::Shape::Type::CIRCLE)))	{
		std::shared_ptr<Sprite> circle = spriteA;
		std::shared_ptr<Sprite> rect = spriteB;
		if (circle->mShape.type != Sprite::Shape::Type::CIRCLE) {
			std::swap(circle, rect);
		}
		float radius =
				std::static_pointer_cast<sf::CircleShape>(circle->mShape.shape)->getRadius();
		sf::Vector2f halfsize = rect->getSize() / 2.0f;
		sf::Vector2f circlePos = circle->getPosition();
		sf::Vector2f rectPos = rect->getPosition();
		// Only circle movement as rectangles don't move.
		sf::Vector2f circleMovement = circle->getSpeed() * (elapsed / 1000.0f);

		// We assume that rectangles are always axis aligned.
		float overlapNoMovementX = Interval(circlePos.x, radius)
								.getOverlap(Interval (rectPos.x, halfsize.x)).getLength();
		float overlapMovementX = Interval(circlePos.x + circleMovement.x, radius)
								.getOverlap(Interval (rectPos.x, halfsize.x)).getLength();
		float overlapNoMovementY = Interval(circlePos.y, radius)
								.getOverlap(Interval (rectPos.y, halfsize.y)).getLength();
		float overlapMovementY = Interval(circlePos.y + circleMovement.y, radius)
								.getOverlap(Interval (rectPos.y, halfsize.y)).getLength();

		bool xyCollisionResult = (((overlapNoMovementX < overlapMovementX) &&
				(overlapNoMovementY > 0)) ||
				((overlapNoMovementY < overlapMovementY) && (overlapNoMovementX > 0)));
		// If circle center is overlapping rectangle on x or y axis, we can take xyCollisionResult.
		if (Interval(rectPos.x, halfsize.x).isInside(circlePos.x) ||
				Interval(rectPos.y, halfsize.y).isInside(circlePos.y)) {
			return xyCollisionResult;
		}
		// Test if the circle is colliding with a corner of the rectangle.
		else if (xyCollisionResult) {
			// This is the same as circle-circle collision.
			sf::Vector2f axis = circle->getPosition() - rect->getPosition();
			// If both objects are at the exact same position, allow any
			// movement for unstucking.
			if (axis == sf::Vector2f()) {
				return true;
			}
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
			return Interval(circlePosProjected, radius)
							.getOverlap(Interval(rectPosProjected, rectHalfWidthProjected))
							.getLength() <
					Interval(circlePosProjected + movementProjected, radius)
							.getOverlap(Interval(rectPosProjected, rectHalfWidthProjected))
							.getLength();
		}
		// If there is no collision on x and y axis, there can't be one at all.
		else {
			return false;
		}
	}
	// Rectangles can't move and thus not collide.
	return false;
}

/**
 * Draws all elements in the group.
 */
void
World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto item : v->second) {
			target.draw(static_cast<sf::Drawable&>(*item), states);
		}
	}
}

/**
 * Creates an interval from a center point and a radius. The interval
 * ranges from center - radius to center + radius.
 */
World::Interval::Interval(float center, float radius) :
		start(center - radius),
		end(center + radius) {
}

/**
 * Returns the overlap between two intervals, e.g. the overlap between
 * intervals (1,3) and (2,4) is (2,3).
 */
World::Interval
World::Interval::getOverlap(Interval other) const {
	if ((start == other.start) && (end == other.end)) {
		return *this;
	}
	Interval smaller = *this;
	Interval bigger = other;
	if (smaller.start > bigger.start) {
		std::swap(smaller, bigger);
	}
	Interval iv(0, 0);
	if (bigger.start < smaller.end) {
		iv.start = bigger.start;
		iv.end = smaller.end;
	}
	else {
		iv.start = iv.end = 0.0f;
	}
	return iv;
}

/**
 * Returns true if the point is inside the interval.
 */
bool
World::Interval::isInside(float point) const {
	return start < point && point < end;
}
/**
 * Returns the length of the interval (distance between start and end).
 */
float
World::Interval::getLength() {
	return end - start;
}
