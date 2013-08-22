/*
 * Circle.h
 *
 *  Created on: 04.05.2013
 *      Author: Felix
 */

#ifndef DG_CIRCLE_H_
#define DG_CIRCLE_H_

#include "CollisionModel.h"
#include "Sprite.h"

class Yaml;

/**
 * Shape that uses a circle as collision model.
 */
class Circle : public CollisionModel, public Sprite {
public:
	explicit Circle(const Vector2f& position, Category category,
			unsigned short mask, const Yaml& config,
			const Vector2f& direction = Vector2f(0, 0));
	virtual ~Circle() = default;

	bool testCollision(std::shared_ptr<Sprite> other,
			Vector2f& offsetFirst, const Vector2f& offsetSecond);
	float getRadius() const;
};

#endif /* DG_CIRCLE_H_ */
