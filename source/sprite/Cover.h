/*
 * Cover.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_COVER_H_
#define DG_COVER_H_

#include "../abstract/Sprite.h"

class Sprite;

/**
 * A wall that can be placed anywhere (not limited by tiles) and have any (rectangular) size.
 */
class Cover : public Sprite {
public:
	Cover(const Vector2f& position, const Vector2i& size, b2World& world);
};

#endif /* DG_COVER_H_ */
