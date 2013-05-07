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

/**
 * Abstract class providing helper functions to test for collisions between shapes.
 */
class CollisionModel {
public:
	virtual ~CollisionModel() = 0;

	static bool testCollision(const Circle& circle, const Rectangle& rect,
			int elapsed);
	static bool testCollision(const Circle& first, const Circle& second,
			int elapsed);
	static bool testCollision(const Rectangle& first, const Rectangle& second,
			int elapsed);
};

#endif /* DG_COLLISIONMODEL_H_ */
