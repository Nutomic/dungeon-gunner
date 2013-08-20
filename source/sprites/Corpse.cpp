/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

#include "../util/Yaml.h"

Corpse::Corpse(const Vector2f& position) :
		Circle(position, CATEGORY_DECORATION, MASK_NONE, Yaml("corpse.yaml")) {
}

