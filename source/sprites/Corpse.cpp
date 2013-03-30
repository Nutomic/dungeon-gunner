/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

Corpse::Corpse(const sf::Vector2f& position, const Yaml& config) :
		Sprite(Data(position, sf::Vector2f(0, 0), CATEGORY_NONSOLID, MASK_NONE),
				config) {
}

