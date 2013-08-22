/*
 * Interval.cpp
 *
 *  Created on: 03.04.2013
 *      Author: Felix
 */

#include "Interval.h"

#include <utility>

Interval::Interval(float start, float end) :
		start(start),
		end(end) {
	if (start > end)
		std::swap(this->start, this->end);
};
/**
 * Creates an interval from a center point and a radius. The interval
 * ranges from center - radius to center + radius.
 */
Interval
Interval::IntervalFromRadius(float center, float radius) {
	return Interval(center - radius, center + radius);
}

/**
 * Creates an Interval from a start and end point.
 */
Interval
Interval::IntervalFromPoints(float start, float end) {
	return Interval(start, end);
}

/**
 * Returns the overlap between two intervals, e.g. the overlap between
 * intervals (1,3) and (2,4) is (2,3).
 */
Interval
Interval::getOverlap(const Interval& other) const {
	Interval iv(0, 0);
	if (other.isInside(start))
		iv.start = start;
	else if (isInside(other.start))
		iv.start = other.start;
	if (other.isInside(end))
		iv.end = end;
	else if (isInside(other.end))
		iv.end = other.end;
	return iv;
}

/**
 * Returns true if the point is inside the interval.
 */
bool
Interval::isInside(float point) const {
	return start <= point && point <= end;
}
/**
 * Returns the length of the interval (distance between start and end).
 */
float
Interval::getLength() {
	return end - start;
}
