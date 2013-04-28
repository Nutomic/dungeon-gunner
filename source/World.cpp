/*
 * World.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "World.h"

#include <Thor/Vectors.hpp>

#include "sprites/TileManager.h"
#include "util/Interval.h"

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
 * Inserts a character into the world. A character can only be inserted once.
 * Also calls insert(character);
 */
void
World::insertCharacter(std::shared_ptr<Character> character) {
#ifndef NDEBUG
	auto item = std::find(mCharacters.begin(), mCharacters.end(), character);
	assert(item == mCharacters.end());
#endif
		mCharacters.push_back(character);
		insert(character);
}

/**
 * Returns all characters that are within maxDistance from position.
 */
std::vector<std::shared_ptr<Character> >
		World::getCharacters(const sf::Vector2f& position, float maxDistance) const {
	std::vector<std::shared_ptr<Character> > visible;
	for (auto it : mCharacters) {
		if (position == it->getPosition())
			continue;
		if (thor::squaredLength(position - it->getPosition()) <=
				maxDistance * maxDistance)
			visible.push_back(it);
	}
	return visible;
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
		for (auto it = v->second.begin(); it != v->second.end(); it++) {
			auto& spriteA = *it;
			sf::Vector2f speed = spriteA->getSpeed() * (elapsed / 1000.0f);
			if (spriteA->getDelete()) {
				v->second.erase(it);
				it--;
			}
			// Apply movement for movable sprites.
			else if (spriteA->getSpeed() != sf::Vector2f()) {
				bool overlap = false;
				for (auto w = mDrawables.begin(); w != mDrawables.end(); w++) {
					for (auto& spriteB : w->second) {
						if (spriteA == spriteB)
							continue;
						// Ignore anything that is filtered by masks.
						if (!spriteA->collisionEnabled(spriteB->getCategory()) ||
								!spriteB->collisionEnabled(spriteA->getCategory()))
							continue;
						if (testCollision(spriteA, spriteB, elapsed)) {
							spriteA->onCollide(spriteB);
							overlap = true;
						}
					}
				}
				if (!overlap)
					spriteA->setPosition(spriteA->getPosition() + speed);
			}
		}
	}
}

/**
 * Calls Character::onThink for each character. Must be called
 * before step so Characters get removed correctly.
 *
 * @param elapsed Time since last call.
 */
void
World::think(int elapsed) {
	for (auto it = mCharacters.begin(); it != mCharacters.end(); ) {
		if ((*it)->getDelete()) {
			mCharacters.erase(it);
			auto& d = mDrawables[Sprite::CATEGORY_ACTOR];
			d.erase(std::find(d.begin(), d.end(), *it));
		}
		else {
			(*it)->onThink(elapsed);
			it++;
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
		if (axis == sf::Vector2f())
			return true;
		axis = thor::unitVector(axis);
		float centerA = thor::dotProduct(axis, spriteA->getPosition());
		float radiusA = spriteA->getRadius();
		float movementA = thor::dotProduct(axis, spriteA->getSpeed() * (elapsed / 1000.0f));
		float centerB = thor::dotProduct(axis, spriteB->getPosition());
		float radiusB = spriteB->getRadius();
		float movementB = thor::dotProduct(axis, spriteB->getSpeed() * (elapsed / 1000.0f));

		// Allow movement if sprites are moving apart.
		return Interval::IntervalFromRadius(centerA, radiusA).getOverlap(
				Interval::IntervalFromRadius(centerB, radiusB)).getLength() <
				Interval::IntervalFromRadius(centerA + movementA, radiusA).getOverlap(
						Interval::IntervalFromRadius(centerB + movementB, radiusB)).getLength();
	}
	// circle-rect collision
	if (((spriteA->mShape.type == Sprite::Shape::Type::CIRCLE) &&
			(spriteB->mShape.type == Sprite::Shape::Type::RECTANGLE)) ||
			((spriteA->mShape.type == Sprite::Shape::Type::RECTANGLE) &&
			(spriteB->mShape.type == Sprite::Shape::Type::CIRCLE)))	{
		std::shared_ptr<Sprite> circle = spriteA;
		std::shared_ptr<Sprite> rect = spriteB;
		if (circle->mShape.type != Sprite::Shape::Type::CIRCLE)
			std::swap(circle, rect);
		float radius = circle->getRadius();
		sf::Vector2f halfsize = rect->getSize() / 2.0f;
		sf::Vector2f circlePos = circle->getPosition();
		sf::Vector2f rectPos = rect->getPosition();
		// Only circle movement as rectangles don't move.
		sf::Vector2f circleMovement = circle->getSpeed() * (elapsed / 1000.0f);

		// We assume that rectangles are always axis aligned.
		float overlapNoMovementX = Interval::IntervalFromRadius(circlePos.x, radius)
								.getOverlap(Interval::IntervalFromRadius(rectPos.x, halfsize.x)).getLength();
		float overlapMovementX = Interval::IntervalFromRadius(circlePos.x + circleMovement.x, radius)
								.getOverlap(Interval::IntervalFromRadius(rectPos.x, halfsize.x)).getLength();
		float overlapNoMovementY = Interval::IntervalFromRadius(circlePos.y, radius)
								.getOverlap(Interval::IntervalFromRadius(rectPos.y, halfsize.y)).getLength();
		float overlapMovementY = Interval::IntervalFromRadius(circlePos.y + circleMovement.y, radius)
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
			sf::Vector2f axis = circle->getPosition() - rect->getPosition();
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
			return Interval::IntervalFromRadius(circlePosProjected, radius)
							.getOverlap(Interval::IntervalFromRadius(rectPosProjected,
									rectHalfWidthProjected))
							.getLength() <
					Interval::IntervalFromRadius(circlePosProjected + movementProjected, radius)
							.getOverlap(Interval::IntervalFromRadius(rectPosProjected,
									rectHalfWidthProjected))
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
	sf::FloatRect screen(target.getViewport(target.getView()));
	screen.left += target.getView().getCenter().x - target.getView().getSize().x / 2;
	screen.top += target.getView().getCenter().y - target.getView().getSize().y / 2;
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto item : v->second) {
			if (item->isInside(screen))
				target.draw(static_cast<sf::Drawable&>(*item), states);
		}
	}
}
