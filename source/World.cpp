/*
 * World.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "World.h"

#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include <Thor/Vectors.hpp>

#include "util/Interval.h"
#include "sprites/TileManager.h"

const float World::WALL_DISTANCE_MULTIPLIER = 1.5f;
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
			   break;
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
	if (item != mCharacters.end())
		   mCharacters.erase(item);
	remove(character);
}

/**
 * Runs the A* path finding algorithm with areas as nodes and portals as edges.
 *
 * @warning Areas and portals must not be changed while this is running.
 *
 * @param start The area to start the path finding from. Must not be null.
 * @param end The goal to reach. Must not be null.
 * @return Path in reverse order (start being the last item and end the first).
 */
std::vector<World::Portal*>
World::astarArea(Area* start, Area* end) const {
	assert(start);
	assert(end);
		
	std::unordered_set<Area*> closed; 
	std::unordered_map<Area*, float> openAreasEstimatedCost; 
	// Navigated areas with previous area/portal.
	std::unordered_map<Area*, std::pair<Area*, Portal*>> previousAreaAndPortal;
	std::unordered_map<Area*, float> bestPathCost;

	openAreasEstimatedCost[start] = heuristic_cost_estimate(start, end);
	bestPathCost[start] = 0;

	while (!openAreasEstimatedCost.empty()) {
		Area* current = std::min_element(openAreasEstimatedCost.begin(),
				openAreasEstimatedCost.end())->first;
		if (current == end) {
			std::vector<Portal*> path;
			auto previous = current;
			while (previous != start) {
				path.push_back(previousAreaAndPortal[previous].second);
				previous = previousAreaAndPortal[previous].first;
			}
			return path;
		}

		openAreasEstimatedCost.erase(current);
		closed.insert(current);
		for (Portal& portal : current->portals) {
			Area* neighbor = portal.area;
			float tentative_g_score = bestPathCost[current] +
					heuristic_cost_estimate(current,neighbor);
			if (closed.find(neighbor) != closed.end()) {
				if (tentative_g_score >= bestPathCost[neighbor])
					continue;
			}

			if ((openAreasEstimatedCost.find(neighbor) ==
					openAreasEstimatedCost.end()) ||
					(tentative_g_score < bestPathCost[neighbor])) {
				previousAreaAndPortal[neighbor] = std::make_pair(current,
						&portal);
				bestPathCost[neighbor] = tentative_g_score;
				openAreasEstimatedCost[neighbor] = bestPathCost[neighbor] +
						heuristic_cost_estimate(neighbor, end);
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
	if (!getArea(end))
		return std::vector<sf::Vector2f>();
	std::vector<Portal*> portals = astarArea(getArea(start), getArea(end));
	if (portals.empty())
		return std::vector<sf::Vector2f>();
	std::vector<sf::Vector2f> path;

	path.push_back(end);
	for (auto p : portals) {
		// Find the point on the line of the portal closest to the previous point.
		sf::Vector2f startToEnd = sf::Vector2f(p->end - p->start);
		float percentage = thor::dotProduct(startToEnd, path.back() - sf::Vector2f(p->start)) /
				thor::squaredLength(startToEnd);
		sf::Vector2f point;

		if (percentage < 0 || percentage > 1.0f) {
			if (thor::squaredLength(sf::Vector2f(p->start) - path.back()) <
					thor::squaredLength(sf::Vector2f(p->end) - path.back())) {
				thor::setLength(startToEnd, WALL_DISTANCE_MULTIPLIER * radius);
				point = sf::Vector2f(p->start) + startToEnd;
			}
			else {
				thor::setLength(startToEnd, WALL_DISTANCE_MULTIPLIER * radius);
				point = sf::Vector2f(p->end) - startToEnd;
			}
		}
		else
			point = sf::Vector2f(p->start) + startToEnd * percentage;

		// Take two points on a line orthogonal to the portal.
		thor::setLength(startToEnd, radius);
		startToEnd = thor::perpendicularVector(startToEnd);
		path.push_back(point + startToEnd);
		path.push_back(point - startToEnd);
		// Make sure the points are in the right order.
		if (thor::squaredLength(*(path.end() - 1) - *(path.end() - 3) ) <
				thor::squaredLength(*(path.end() - 2) - *(path.end() - 3) ))
			std::swap(*(path.end() - 1), *(path.end() - 2));
	}
	return path;
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
 * Initializes start and end of an area, sets area to null.
 */
World::Portal::Portal(const sf::Vector2i& start, const sf::Vector2i& end) :
	start(start),
	end(end),
	area(nullptr) {
}

/**
 * Returns the linear distance between two areas (using their center).
 */
float
World::heuristic_cost_estimate(Area* start, Area* end) const {
	return thor::length(sf::Vector2f(end->center - start->center));
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
			auto spriteA = *it;
			if (spriteA->getDelete()) {
				remove(spriteA);
				it--;
			}
			// Apply movement for movable sprites.
			else if ((*it)->getSpeed() != sf::Vector2f()) {
				sf::Vector2f speed = spriteA->getSpeed();
				speed *= elapsed / 1000.0f;
				bool overlap = false;
				for (auto w = mDrawables.begin(); w != mDrawables.end(); w++) {
					for (auto spriteB : w->second) {
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
 * before step (so Characters get removed asap).
 *
 * @param elapsed Time since last call.
 */
void
World::think(int elapsed) {
	for (auto it = mCharacters.begin(); it != mCharacters.end(); ) {
		if ((*it)->getDelete())
			removeCharacter(*it);
		else {
			(*it)->onThink(elapsed);
			it++;
		}
	}
}

/**
 * Inserts an area used for path finding.
 *
 * @parm rect Rectangle the area covers.
 */
void
World::insertArea(const sf::IntRect& rect) {
	Area a;
	// Not sure why the offset of -50 is required, but with it, areas align
	// with tiles perfectly.
	a.area = sf::IntRect(rect.left * TileManager::TILE_SIZE.x - 50,
			rect.top * TileManager::TILE_SIZE.y - 50,
			rect.width * TileManager::TILE_SIZE.x,
			rect.height * TileManager::TILE_SIZE.y);
	a.center = sf::Vector2i(a.area.left + a.area.width / 2,
			a.area.top + a.area.height / 2);
	mAreas.push_back(a);
}

/**
 * Generates portals that connect areas. Needs to be run after insertArea for
 * path finding to work.
 *
 * Could be improved by only checking nearby areas.
 */
void
World::generatePortals() {
	for (Area& it : mAreas) {
		// We currently recreate portals for all existing areas, so we have
		// to clear in case this was already generated.
		it.portals.clear();
		for (Area& other : mAreas) {
			if (&it == &other)
				continue;
			Portal portal;
			portal.area = &other;
			if (it.area.left + it.area.width == other.area.left) {
				Interval overlap = Interval::IntervalFromPoints(it.area.top,
						it.area.top + it.area.height)
						.getOverlap(Interval::IntervalFromPoints(other.area.top,
								other.area.top + other.area.height));
				if (overlap.getLength() > 0) {
					portal.start = sf::Vector2i(other.area.left, overlap.start);
					portal.end = sf::Vector2i(other.area.left, overlap.end);
					it.portals.push_back(portal);
				}
			}
			if (other.area.left + other.area.width == it.area.left) {
				Interval overlap = Interval::IntervalFromPoints(it.area.top,
						it.area.top + it.area.height)
						.getOverlap(Interval::IntervalFromPoints(other.area.top,
								other.area.top + other.area.height));
				if (overlap.getLength() > 0) {
					portal.start = sf::Vector2i(it.area.left, overlap.start);
					portal.end = sf::Vector2i(it.area.left, overlap.end);
					it.portals.push_back(portal);
				}
			}
			else if (it.area.top + it.area.height == other.area.top) {
				Interval overlap = Interval::IntervalFromPoints(it.area.left,
						it.area.left + it.area.width)
						.getOverlap(Interval::IntervalFromPoints(other.area.left,
								other.area.left + other.area.width));
				if (overlap.getLength() > 0) {
					portal.start = sf::Vector2i(overlap.start, other.area.top);
					portal.end = sf::Vector2i(overlap.end, other.area.top);
					it.portals.push_back(portal);
				}
			}
			else if (other.area.top + other.area.height == it.area.top) {
				Interval overlap = Interval::IntervalFromPoints(it.area.left,
						it.area.left + it.area.width)
						.getOverlap(Interval::IntervalFromPoints(other.area.left,
								other.area.left + other.area.width));
				if (overlap.getLength() > 0) {
					portal.start = sf::Vector2i(overlap.start, it.area.top);
					portal.end = sf::Vector2i(overlap.end, it.area.top);
					it.portals.push_back(portal);
				}
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
 * Returns the area where point is in.
 * Just iterates through all areas and tests each.
 */
World::Area*
World::getArea(const sf::Vector2f& point) const {
	for (auto area = mAreas.begin(); area != mAreas.end(); area++) {
		if (sf::FloatRect(area->area).contains(point))
			// Make the return value non-const for convenience.
			return &const_cast<Area&>(*area);
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
