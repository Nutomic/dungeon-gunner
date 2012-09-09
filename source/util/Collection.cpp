/*
 * Collection.cpp
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#include "Collection.h"

#include <algorithm>

/**
 * Insert a drawable into the group. Drawables should only be handled with shared_ptr.
 * An object can't be inserted more than once at the same level.
 */
void
Collection::insert(std::shared_ptr<Sprite> drawable, Level level) {
	auto item = std::find(mDrawables[level].begin(), mDrawables[level].end(), drawable);
	if (item == mDrawables[level].end()) {
		mDrawables[level].push_back(drawable);
	}
}

/**
 * Removes a drawable from the group.
 */
void
Collection::remove(std::shared_ptr<Sprite> drawable) {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		auto item = std::find(v->second.begin(), v->second.end(), drawable);
		if (item != v->second.end()) {
			   v->second.erase(item);
		}
	}
}

/**
 * Deletes any sprites which return true for getDelete().
 */
void
Collection::checkDelete() {
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
Collection::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto v = mDrawables.begin(); v != mDrawables.end(); v++) {
		for (auto item : v->second) {
			target.draw(static_cast<sf::Drawable&>(*item), states);
		}
	}
}

