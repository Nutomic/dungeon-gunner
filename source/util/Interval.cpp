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
	if (this->start > this->end) {
		std::swap(this->start, this->end);
	}
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
Interval::getOverlap(Interval other) const {
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
Interval::isInside(float point) const {
	return start < point && point < end;
}
/**
 * Returns the length of the interval (distance between start and end).
 */
float
Interval::getLength() {
	return end - start;
}
