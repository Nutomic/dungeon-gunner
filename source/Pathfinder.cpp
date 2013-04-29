/*
 * Pathfinder.cpp
 *
 *  Created on: 28.04.2013
 *      Author: Felix
 */

#include "Pathfinder.h"

#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include <Thor/Vectors.hpp>

#include "util/Interval.h"
#include "sprites/Tile.h"

const float Pathfinder::WALL_DISTANCE_MULTIPLIER = 1.5f;

/**
 * Runs the A* path finding algorithm with areas as nodes and portals as edges.
 *
 * @warning Areas and portals must not be changed while this is running.
 *
 * @param start The area to start the path finding from. Must not be null.
 * @param end The goal to reach. Must not be null.
 * @return Path in reverse order (start being the last item and end the first).
 */
std::vector<Pathfinder::Portal*>
Pathfinder::astarArea(Area* start, Area* end) const {
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
Pathfinder::getPath(const sf::Vector2f& start, const sf::Vector2f& end,
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
 * Returns the linear distance between two areas (using their center).
 */
float
Pathfinder::heuristic_cost_estimate(Area* start, Area* end) const {
	return thor::length(sf::Vector2f(end->center - start->center));
}

bool Pathfinder::Portal::operator==(const Portal& p) {
    return start == p.start && end == p.end && area == p.area;
}

/**
 * Inserts an area used for path finding.
 *
 * @parm rect Rectangle the area covers.
 */
void
Pathfinder::insertArea(const sf::IntRect& rect) {
	Area a;
	// Not sure why the offset of -50 is required, but with it, areas align
	// with tiles perfectly.
	a.area = sf::IntRect(rect.left * Tile::TILE_SIZE.x - 50,
			rect.top * Tile::TILE_SIZE.y - 50,
			rect.width * Tile::TILE_SIZE.x,
			rect.height * Tile::TILE_SIZE.y);
	a.center = sf::Vector2i(a.area.left + a.area.width / 2,
			a.area.top + a.area.height / 2);
	mAreas.push_back(a);
}

/**
 * Generates portals that connect areas. Needs to be run after insertArea for
 * path finding to work.
 */
void
Pathfinder::generatePortals() {
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
 * Returns the area where point is in.
 */
Pathfinder::Area*
Pathfinder::getArea(const sf::Vector2f& point) const {
	for (auto& area : mAreas) {
		if (sf::FloatRect(area.area).contains(point))
			// Make the return value non-const for convenience.
			return &const_cast<Area&>(area);
	}
	return nullptr;
}
