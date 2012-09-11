/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include "../abstract/Actor.h"
#include "../abstract/Sprite.h"

#include "../util/Vector.h"

class Enemy : public Sprite, public Actor {
// Public functions.
public:
	Enemy(b2World& world, const Vector2f& position);
	~Enemy();

// Private functions.
private:
	void onThink(float elapsedTime);
};

#endif /* DG_ENEMY_H_ */