/*
 * Interval.h
 *
 *  Created on: 03.04.2013
 *      Author: Felix
 */

#ifndef DG_INTERVAL_H_
#define DG_INTERVAL_H_

class Interval {
public:
	static Interval IntervalFromRadius(float center, float radius);
	static Interval IntervalFromPoints(float start, float end);
	Interval getOverlap(Interval other) const;
	bool isInside(float point) const;
	float getLength();

public:
	float start;
	float end;

private:
	Interval(float start, float end);
};

#endif /* DG_INTERVAL_H_ */
