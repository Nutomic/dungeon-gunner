/*
 * RotatingShield.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_ROTATINGSHIELD_H_
#define DG_ROTATINGSHIELD_H_

#include "../abstract/Rectangle.h"

class RotatingShield : public Rectangle {
public:
	explicit RotatingShield(const sf::Vector2f& position);

private:
	using Sprite::setDelete;
	using Sprite::setDirection;
	using Sprite::getPosition;

	friend class Shield;
};

#endif /* DG_ROTATINGSHIELD_H_ */
