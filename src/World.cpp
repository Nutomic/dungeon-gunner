/*
 * World.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "World.h"

#include <Thor/Vectors.hpp>

#include "sprites/Tile.h"
#include "util/Interval.h"
#include "util/Log.h"

/**
 * Insert a drawable into the group. Drawables should only be handled with shared_ptr.
 * An object can't be inserted more than once at the same level.
 */
void
World::insert(std::shared_ptr<Sprite> drawable) {
#ifndef RELEASE
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
#ifndef RELEASE
	auto item = std::find(mCharacters.begin(), mCharacters.end(), character);
	assert(item == mCharacters.end());
#endif
	mCharacters.push_back(character);
	insert(character);
}

void
World::remove(std::shared_ptr<Sprite> drawable) {
	Sprite::Category cat = drawable->getCategory();
	auto item = std::find(mDrawables[cat].begin(), mDrawables[cat].end(), drawable);
	mDrawables[cat].erase(item);
}

/**
 * Returns all characters that are within maxDistance from position.
 */
std::vector<std::shared_ptr<Character> >
World::getCharacters(const Vector2f& position, float maxDistance) const {
	std::vector<std::shared_ptr<Character> > visible;
	for (const auto& it : mCharacters) {
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
		for (auto it = v->second.begin(); it != v->second.end(); ) {
			if ((*it)->getDelete() && (*it)->getCategory() != Character::CATEGORY_ACTOR)
				it = v->second.erase(it);
			else {
				// Don't run collision tests if sprite is not moving.
				if ((*it)->getSpeed() != Vector2f())
					applyMovement(*it, elapsed);
				it++;
			}
		}
	}
}

/**
 * Tests spriteA for overlap with every other sprite (considering collision
 * masks).
 */
void
World::applyMovement(std::shared_ptr<Sprite> sprite, int elapsed) {
	Vector2f offset = sprite->getSpeed() * (elapsed / 1000.0f);
	for (auto w = mDrawables.begin(); w != mDrawables.end(); w++) {
		for (const auto& other : w->second) {
			if (sprite == other)
				continue;
			// Ignore anything that is filtered by masks.
			if (!sprite->collisionEnabled(other->getCategory()) ||
					!other->collisionEnabled(sprite->getCategory()))
				continue;
			if (sprite->testCollision(other, offset,
					other->getSpeed() * (elapsed / 1000.0f))) {
				sprite->onCollide(other);
				other->onCollide(sprite);
			}
		}
	}
	sprite->setPosition(sprite->getPosition() + offset);
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
		auto value = *it;
		if (value->getDelete()) {
			remove(value);
			it = mCharacters.erase(it);
		}
		else {
			value->onThink(elapsed);
			it++;
		}
	}
}

/**
 * Draws all elements in the group.
 */
void
World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::FloatRect screen(target.getViewport(target.getView()));
	screen.left += target.getView().getCenter().x - target.getView().getSize().x / 2;
	screen.top += target.getView().getCenter().y - target.getView().getSize().y / 2;
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++)
		for (const auto& item : v->second)
			if (item->isInside(screen))
				target.draw(static_cast<sf::Drawable&>(*item), states);
}

/*
 * Performs a raycast between two points to check if the path between them is
 * clear of walls. Does not consider characters, bullets etc.
 *
 * @param lineStart First point of the line to test.
 * @param lineEnd Second point of the line to test.
 * @return True if the ray was not blocked.
 */
bool
World::raycast(const Vector2f& lineStart,
		const Vector2f& lineEnd) const {
	assert(lineStart != lineEnd);
	Vector2f lineCenter = lineStart + 0.5f * (lineEnd - lineStart);
	for (const auto& it : mDrawables.at(Sprite::Category::CATEGORY_WORLD)) {
		if (!it->collisionEnabled(Sprite::CATEGORY_ACTOR))
			continue;
		Vector2f axis = it->getPosition() - lineCenter;
		if (axis == Vector2f())
			return false;

		axis = thor::unitVector(axis);
		Vector2f halfsize = it->getSize() / 2.0f;
		float rectPosProjected = thor::dotProduct(axis, it->getPosition());
		float lineStartProjected = thor::dotProduct(axis, lineStart);
		float lineEndProjected = thor::dotProduct(axis, lineEnd);
		// For corner projections, those on the same line with the rect
		// center are equal by value, so we only need one on each axis
		// and take the maximum.
		float rectHalfWidthProjected = std::max(
				abs(thor::dotProduct(axis, halfsize)),
				abs(thor::dotProduct(axis,
						Vector2f(halfsize.x, -halfsize.y))));
		Interval line = Interval::IntervalFromPoints(lineStartProjected,
				lineEndProjected);
		Interval rect = Interval::IntervalFromRadius(rectPosProjected,
				rectHalfWidthProjected);
		// Allow movement if sprites are moving apart.
		if (line.getOverlap(rect).getLength() > 0.0f)
			return false;
	}
	return true;
}

/**
 * Returns all sprites that are at most distance pixels away from position.
 */
std::vector<std::shared_ptr<Sprite> >
World::getNearbySprites(const Vector2f& position, float distance) const {
	std::vector<std::shared_ptr<Sprite> > ret;
	for (const auto& v : mDrawables)
		for (const auto& d : v.second)
			if (thor::squaredLength(d->getPosition() - position) <= distance * distance)
				ret.push_back(d);
	return ret;
}

/**
 * Returns the item closest to position, or null if it is further than
 * Character::ITEM_PICKUP_MAX_DISTANCE away.
 */
std::shared_ptr<Item>
World::getClosestItem(const Vector2f& position) const {
	float distance = std::numeric_limits<float>::max();
	std::shared_ptr<Item> closest;
	for (auto& s : getNearbySprites(position, Character::ITEM_PICKUP_MAX_DISTANCE)) {
		std::shared_ptr<Item> converted = std::dynamic_pointer_cast<Item>(s);
		if (converted.get() != nullptr &&
				thor::squaredLength(position - converted->getPosition()) < distance * distance) {
			closest = converted;
			distance = thor::squaredLength(position - converted->getPosition());
		}
	}
	return (distance <= Character::ITEM_PICKUP_MAX_DISTANCE * Character::ITEM_PICKUP_MAX_DISTANCE)
			? closest
			: std::shared_ptr<Item>();
}
