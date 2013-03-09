/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include <SFML/System.hpp>

#include "../abstract/Character.h"
#include "../World.h"
#include "../util/Yaml.h"

class Character;
class World;
class Instances;
class Yaml;

class Enemy : public Character {
// Public functions.
public:
	Enemy(World& collection, Pathfinder& pathfinder,
			const sf::Vector2f& position, const Yaml& config);
};

#endif /* DG_ENEMY_H_ */
