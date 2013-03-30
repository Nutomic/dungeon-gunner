/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

Corpse::Corpse(const sf::Vector2f& position, const Yaml& config) :
		Sprite(Data(position, CATEGORY_NONSOLID, MASK_NONE), config) {
}

