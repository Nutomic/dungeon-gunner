/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include "../abstract/Character.h"
#include "../abstract/Sprite.h"
#include "../util/Collection.h"
#include "../util/Vector.h"

class Character;
class Sprite;
class Collection;

class Enemy : public Sprite, public Character {
// Public functions.
public:
	Enemy(b2World& world, const Vector2f& position, Collection& collection);
	~Enemy();

// Private functions.
private:
	void onThink(float elapsedTime);
	void onDeath();

// Private variablese.
private:
	b2World& mWorld;
	Collection& mCollection;
};

#endif /* DG_ENEMY_H_ */
