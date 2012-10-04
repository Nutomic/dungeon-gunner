/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include "../Instances.h"
#include "../abstract/Character.h"
#include "../util/Collection.h"
#include "../util/Vector.h"

class Character;
class Collection;
class Instances;

class Enemy : public Character {
// Public functions.
public:
	Enemy(const Instances& instances, const Vector2f& position);
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
