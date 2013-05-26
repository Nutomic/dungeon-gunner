/*
 * CollisionModel.h
 *
 *  Created on: 07.05.2013
 *      Author: Felix
 */

#ifndef DG_COLLISIONMODEL_H_
#define DG_COLLISIONMODEL_H_

class Circle;
class Rectangle;

#include <SFML/System.hpp>

/**
 * Abstract class providing helper functions to test for collisions between shapes.
 *
 * http://www.metanetsoftware.com/technique/tutorialA.html
 */
class CollisionModel {
public:
	virtual ~CollisionModel() = 0;

protected:
	static bool testCollision(const Circle& circle, const Rectangle& rect,
			sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond);
	static bool testCollision(const Circle& first, const Circle& second,
			sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond);
	static bool testCollision(const Rectangle& first, const Rectangle& second,
			sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond);
};

#endif /* DG_COLLISIONMODEL_H_ */
