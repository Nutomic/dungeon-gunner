/*
 * Item.h
 *
 *  Created on: 13.07.2013
 *      Author: Felix
 */

#ifndef DG_ITEM_H_
#define DG_ITEM_H_

#include "../abstract/Sprite.h"

class World;

class Item : public Sprite {
public:
	Item(const Vector2f& size, const std::string& texture);
	virtual ~Item() {};

	virtual std::string getName() const = 0;
	void drop(const Vector2f& position);
	bool testCollision(std::shared_ptr<Sprite> other,
			Vector2f& offsetFirst, const Vector2f& offsetSecond);
};

#endif /* DG_ITEM_H_ */
