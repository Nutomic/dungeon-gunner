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
// Public functions.
public:
	Enemy(World& collection, const sf::Vector2f& position, const Yaml& config);
};

#endif /* DG_ENEMY_H_ */
