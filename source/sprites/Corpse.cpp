/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

Corpse::Corpse(b2World& world, const Vector2f& position, const Yaml& config) :
		Sprite(config, PhysicalData(position, world, CATEGORY_NONSOLID, MASK_NONE, false)) {
}

