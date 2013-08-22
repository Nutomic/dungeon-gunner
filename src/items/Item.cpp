/*
 * Item.cpp
 *
 *  Created on: 13.07.2013
 *      Author: Felix
 */

#include "Item.h"

#include "../World.h"

Item::Item(const Vector2f& size, const std::string& texture) :
		Sprite(Vector2f(), CATEGORY_NONSOLID, MASK_NONE, size, texture,
				Vector2f()) {
}

void
Item::drop(const Vector2f& position) {
	setPosition(position);
}

bool
Item::testCollision(std::shared_ptr<Sprite> other,
		Vector2f& offsetFirst, const Vector2f& offsetSecond) {
	return false;
}
