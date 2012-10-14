/*
 * Body.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Body.h"

Body::Body(b2World& world, const Vector2f& position, const Yaml& config) :
		Sprite(config, PhysicalData(position, world, CATEGORY_NONSOLID, MASK_NONE, false)) {
}

