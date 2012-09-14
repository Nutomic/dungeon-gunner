/*
 * Body.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Body.h"

Body::Body(b2World& world, const Vector2f& position) :
		Sprite("body.png", PhysicalData(position, Vector2i(50, 50), world,
			   CATEGORY_NONSOLID, MASK_NONE, false)) {
}

