/*
 * Cover.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Cover.h"

Cover::Cover(const Vector2f& position, const Vector2i& size, b2World& world, const Yaml& config) :
		Sprite("cover.png", PhysicalData(position, size, world, CATEGORY_WORLD, MASK_ALL, false)) {
}

