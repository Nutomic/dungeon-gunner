/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include "../sprites/abstract/Character.h"

class World;

class Enemy : public Character {
public:
	explicit Enemy(World& world, Pathfinder& pathfinder,
			const Vector2f& position);

private:
	virtual void onThink(int elapsed) override;
};

#endif /* DG_ENEMY_H_ */
