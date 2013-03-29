/*
 * World.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "World.h"

#include <unordered_set>
#include <unordered_map>

#include <Thor/Vectors.hpp>

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
 * Removes a character from the world.
 * Also calls remove(character);
 */
void
World::removeCharacter(std::shared_ptr<Character> character) {
	auto item = std::find(mCharacters.begin(), mCharacters.end(), character);
	if (item != mCharacters.end()) {
		   mCharacters.erase(item);
	}
	remove(character);
}

/**
 * Generate path finding base data.
 *
 * Hardcoded as heuristic may be unnecessary with proper map generation.
 *
 * @warning Must not be run while getPath() is running (raw pointers).
 */
void
World::generateAreas() {
	Area a;

	a.area = sf::FloatRect(50, 50, 900, 300);
	a.center = sf::Vector2f(500, 200);
	mAreas.push_back(a);
	a.area = sf::FloatRect(450, 350, 450, 100);
	a.center = sf::Vector2f(675, 400);
	mAreas.push_back(a);
	a.area = sf::FloatRect(50, 450, 900, 500);
	a.center = sf::Vector2f(500, 700);
	mAreas.push_back(a);

	Portal p1;
	Portal p2;
	std::vector<Portal> vp;

	p1.start = sf::Vector2f(450, 350);
	p1.end = sf::Vector2f(950, 350);
	p1.area = &mAreas[1];
	vp.push_back(p1);
	mAreas[0].portals = vp;

	vp.clear();
	p2.start = sf::Vector2f(450, 450);
	p2.end = sf::Vector2f(950, 450);
	p2.area = &mAreas[1];
	vp.push_back(p2);
	mAreas[2].portals = vp;

	vp.clear();
	p1.area = &mAreas[0];
	vp.push_back(p1);
	p2.area = &mAreas[2];
	vp.push_back(p2);
	mAreas[1].portals = vp;
}

/**
 * Runs the A* path finding algorithm with areas as nodes and portals as edges.
 *
 * @warning Areas and portals must not be changed while this is running.
 *
 * @param start The area to start the path finding from. Must not be null.
 * @param end The goal to reach. May be null.
 * @return Path in reverse order (start being the last item and end the first).
 */
std::vector<World::Portal*>
World::astarArea(Area* start, Area* end) const {
	assert(start);
	if (!end) {
		return std::vector<World::Portal*>();
	}

	std::unordered_set<Area*> closedset; // The set of nodes already evaluated.
	// Set of nodes to be evaluated, with corresponding estimated cost start -> area -> goal
	std::unordered_map<Area*, float> openset;
	// The map of navigated nodes, with previous, lowest cost Area/Portal.
	std::unordered_map<Area*, std::pair<Area*, Portal*>> came_from;
	std::unordered_map<Area*, float> g_score; // Cost from start along best known path.

	openset[start] = heuristic_cost_estimate(start, end);
	g_score[start] = 0;

	while (!openset.empty()) {
		// the node in openset having the lowest f_score value.
		Area* current = std::min_element(openset.begin(), openset.end())->first;
		if (current == end) {
			std::vector<Portal*> path;
			auto previous = current;
			while (previous != start) {
				path.push_back(came_from[previous].second);
				previous = came_from[previous].first;
			}
			return path;
		}

		openset.erase(current);
		closedset.insert(current);
		for (Portal& portal : current->portals) {
			Area* neighbor = portal.area;
			// Use edge weight instead of heuristic cost estimate?
			float tentative_g_score = g_score[current] + heuristic_cost_estimate(current,neighbor);
			if (closedset.find(neighbor) != closedset.end()) {
				if (tentative_g_score >= g_score[neighbor]) {
					continue;
				}
			}

			if ((openset.find(neighbor) == openset.end()) || (tentative_g_score < g_score[neighbor])) {
				came_from[neighbor] = std::make_pair(current, &portal);
				g_score[neighbor] = tentative_g_score;
				openset[neighbor] = g_score[neighbor] + heuristic_cost_estimate(neighbor, end);
			}
		}
	}
	return std::vector<Portal*>();
}

/**
 * Returns path in reverse order.
 *
 * @warning Areas and portals must not be changed while this running.
 *
 * @param start Position to start the path from.
 * @param end Position to move to.
 * @param radius Radius of the moving object.
 * @return Path from end to start (path from start to end in reverse order).
 */
std::vector<sf::Vector2f>
World::getPath(const sf::Vector2f& start, const sf::Vector2f& end,
		float radius) const {
	std::vector<Portal*> portals = astarArea(getArea(start), getArea(end));
	std::vector<sf::Vector2f> path;

	path.push_back(end);
	for (auto p : portals) {
		// Find the point on the line of the portal closest to the previous point.
		sf::Vector2f startToEnd = p->end - p->start;
		float percentage = thor::dotProduct(startToEnd, path.back() - p->start) /
				thor::squaredLength(startToEnd);
		sf::Vector2f point;

		if (percentage < 0 || percentage > 1.0f) {
			if (thor::squaredLength(p->start - path.back()) <
					thor::squaredLength(p->end - path.back())) {
				thor::setLength(startToEnd, radius);
				point = p->start + startToEnd;
			}
			else {
				thor::setLength(startToEnd, radius);
				point = p->end - startToEnd;
			}
		}
		else {
			point = p->start + startToEnd * percentage;
		}

		// Take two points on a line orthogonal to the portal.
		thor::setLength(startToEnd, radius);
		startToEnd = thor::perpendicularVector(startToEnd);
		path.push_back(point + startToEnd);
		path.push_back(point - startToEnd);
		// Make sure the points are in the right order.
		if (thor::squaredLength(*(path.end() - 1) - *(path.end() - 3) ) <
				thor::squaredLength(*(path.end() - 2) - *(path.end() - 3) )) {
			std::swap(*(path.end() - 1), *(path.end() - 2));
		}
	}
	return path;
}

/**
 * Returns the linear distance between two areas (using their center).
 */
float
World::heuristic_cost_estimate(Area* start, Area* end) const {
	return thor::length(end->center - start->center);
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
 * Calls Character::onThink for each character.
 *
 * @param elapsed Time since last call.
 */
void
World::think(int elapsed) {
	for (auto it : mCharacters) {
		it->onThink(elapsed);
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
		float radiusA = spriteA->getRadius();
		float movementA = thor::dotProduct(axis, spriteA->getSpeed() * (elapsed / 1000.0f));
		float centerB = thor::dotProduct(axis, spriteB->getPosition());
		float radiusB = spriteB->getRadius();
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
		float radius = circle->getRadius();
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
 * Returns the area where point is in.
 * Just iterates through all areas and tests each.
 */
World::Area*
World::getArea(const sf::Vector2f& point) const {
	for (auto area = mAreas.begin(); area != mAreas.end(); area++) {
		if (area->area.contains(point)) {
			// Make the return value non-const for convenience.
			return &const_cast<Area&>(*area);
		}
	}
	return nullptr;
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
