/*
 * Corpse.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#include "Corpse.h"

#include "../util/Yaml.h"

const std::string Corpse::CONFIG = "corpse.yaml";

Corpse::Corpse(const sf::Vector2f& position) :
		Circle(position, CATEGORY_NONSOLID, MASK_NONE, Yaml(CONFIG)) {
}

