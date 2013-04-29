/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include "../abstract/Character.h"

class World;
class Yaml;

class Enemy : public Character {
public:
	explicit Enemy(World& world, Pathfinder& pathfinder,
			const sf::Vector2f& position, const Yaml& config);

protected:
	virtual void onThink(int elapsed);
};

#endif /* DG_ENEMY_H_ */
