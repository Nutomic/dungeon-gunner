/*
 * Rectangle.h
 *
 *  Created on: 04.05.2013
 *      Author: Felix
 */

#ifndef DG_RECTANGLE_H_
#define DG_RECTANGLE_H_

#include "CollisionModel.h"
#include "Sprite.h"

class Yaml;

/**
 * Shape that uses an axis aligned Rectangle as a collision model.
 */
class Rectangle : public CollisionModel, public Sprite {
public:
	explicit Rectangle(const sf::Vector2f& position, Category category,
			unsigned short mask, const Yaml& config,
			const sf::Vector2f& direction = sf::Vector2f(0, 0));
	virtual ~Rectangle() = default;

	bool testCollision(std::shared_ptr<Sprite> other,
			sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond);
};

#endif /* DG_RECTANGLE_H_ */
