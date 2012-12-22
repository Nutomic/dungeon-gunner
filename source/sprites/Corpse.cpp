/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

Corpse::Corpse(const Vector2f& position, const Yaml& config) :
		Sprite(config, Data(position, 0, CATEGORY_NONSOLID, MASK_NONE)) {
}

