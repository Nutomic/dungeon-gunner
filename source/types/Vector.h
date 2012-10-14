/*
 * Vector.h
 *
 *  Created on: 03.08.2012
 *      Author: Felix
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

#include <SFML/System.hpp>

#include <Box2D/Box2D.h>

#include <Thor/Vectors.hpp>

/**
 * 2D floating point vector with x/y members.
 */
typedef sf::Vector2f Vector2f;

/**
 * 2D integer vector with x/y members.
 */
typedef sf::Vector2i Vector2i;

/**
 * Constant for conversion between Box2D vectors and SFML vectors.
 */
static const int PIXELS_PER_METER = 25;

/**
 * Converts a distance from pixels to meters.
 */
inline float
pixelToMeter(float in) {
	return in / PIXELS_PER_METER;
}

/**
 * Converts a distance from meters to pixels.
 */
inline float
meterToPixel(float in) {
	return in * PIXELS_PER_METER;
}

/**
 * Converts Box2D metric vector to SFML pixel vector.
 */
inline Vector2f
vector(const b2Vec2& in) {
	return Vector2f(meterToPixel(in.x), meterToPixel(in.y));
}

/**
 * Converts SFML pixel vector to Box2D metric vector.
 */
inline b2Vec2
vector(const Vector2f& in) {
	return b2Vec2(pixelToMeter(in.x), pixelToMeter(in.y));
}

/**
 * Converts a vector to an SFML angle with the same direction.
 */
inline float
angle(Vector2f in) {
	return 180 - thor::toDegree(atan2(in.x, in.y));
}

/**
 * Converts an SFML angle to a unit vector with the same direction.
 */
inline Vector2f
angle(float in) {
	in = thor::toRadian(180 - in);
	return Vector2f(sin(in), cos(in));
}


#endif /* VECTOR_H_ */
