/*
 * Item.cpp
 *
 *  Created on: 13.07.2013
 *      Author: Felix
 */

#include "Item.h"

#include "../World.h"

Item::Item(const sf::Vector2f& size, const std::string& texture) :
		Sprite(sf::Vector2f(), CATEGORY_NONSOLID, MASK_NONE, size, texture,
				sf::Vector2f()) {
}

void
Item::drop(const sf::Vector2f& position) {
	setPosition(position);
}

bool
Item::testCollision(std::shared_ptr<Sprite> other,
		sf::Vector2f& offsetFirst, const sf::Vector2f& offsetSecond) {
	return false;
}
