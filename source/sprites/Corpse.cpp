/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

Corpse::Corpse(World& world, const Vector2f& position, const Yaml& config) :
		Sprite(config, Data(world, position, 0, CATEGORY_NONSOLID, MASK_NONE)) {
}

