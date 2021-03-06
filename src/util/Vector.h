/*
 * Vector.h
 *
 *  Created on: 07.08.2013
 *      Author: Felix
 */

#ifndef DG_VECTOR_H_
#define DG_VECTOR_H_

#include <SFML/System.hpp>

#include <LTBL/Constructs/Vec2f.h>

/**
 * Vector class with comparison operator. All other operators are inherited
 * from sf::Vector2.
 */
template <typename T>
class Vector2 : public sf::Vector2<T> {
public:
	/// Needed for conversion to Vec2f.
	static int SCREEN_HEIGHT;

	Vector2() : sf::Vector2<T>() {};
	Vector2(T x, T y) : sf::Vector2<T>(x, y) {};
    /**
     * Implicitly catches and converts any vectors that have been converted to
     * sf::Vector2 (as done by arithmethic operations defined by sf::Vector2).
     */
	template <typename U>
    Vector2(const sf::Vector2<U>& vector) : sf::Vector2<T>(vector) {};

	Vec2f
	toVec2f() {
		return Vec2f(sf::Vector2f::x, SCREEN_HEIGHT - sf::Vector2f::y);
	}
};

template <typename T>
int Vector2<T>::SCREEN_HEIGHT = 0;

/**
 * Comparison operator meant for containers like std::set.
 *
 * Provides a clearly defined, but otherwise meaningless sorting of vectors.
 */
template <typename T>
bool operator <(const Vector2<T>& left, const Vector2<T>& right) {
	return left.x < right.x || (left.x == right.x && left.y < right.y);
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

#endif /* DG_VECTOR_H_ */
