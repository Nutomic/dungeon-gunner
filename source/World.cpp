/*
 * World.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "World.h"

#include <algorithm>

#include <Thor/Vectors.hpp>

/**
 * Insert a drawable into the group. Drawables should only be handled with shared_ptr.
 * An object can't be inserted more than once at the same level.
 */
void
World::insert(std::shared_ptr<Sprite> drawable) {
	Body::Category cat = drawable->getCategory();
	auto item = std::find(mDrawables[cat].begin(), mDrawables[cat].end(), drawable);
	if (item == mDrawables[cat].end()) {
		mDrawables[cat].push_back(drawable);
	}
}

/**
 * Removes a drawable from the group.
 */
void
World::remove(std::shared_ptr<Sprite> drawable) {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		auto item = std::find(v->second.begin(), v->second.end(), drawable);
		if (item != v->second.end()) {
			   v->second.erase(item);
		}
	}
}

void
World::step(int elapsed) {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto item = v->second.begin(); item != v->second.end(); item++) {
			sf::Vector2f speed = (*item)->getSpeed();
			speed *= elapsed / 1000.0f;
			(*item)->setPosition((*item)->getPosition() + speed);
		}
	}
}

/**
 * Deletes any sprites which return true for getDelete().
 */
void
World::checkDelete() {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto item = v->second.begin(); item != v->second.end(); ) {
			if ((*item)->getDelete()) {
				item = v->second.erase(item);
			}
			else {
				item++;
			}
		}
	}
}

/**
 * Draws all elements in the group.
 */
void
World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto item : v->second) {
			target.draw(static_cast<sf::Drawable&>(*item), states);
		}
	}
}

