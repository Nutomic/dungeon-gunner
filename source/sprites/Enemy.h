/*
 * Enemy.h
 *
 *  Created on: 10.09.2012
 *      Author: Felix
 */

#ifndef DG_ENEMY_H_
#define DG_ENEMY_H

#include "../types/Instances.h"
#include "../abstract/Character.h"
#include "../util/Collection.h"
#include "../types/Vector.h"
#include "../util/Yaml.h"

class Character;
class Collection;
class Instances;
class Yaml;

class Enemy : public Character {
// Public functions.
public:
	Enemy(const Instances& instances, const Vector2f& position, const Yaml& config);

// Private functions.
private:
	void onThink(float elapsedTime);

// Private variablese.
private:
	World& mWorld;
	Collection& mCollection;
};

#endif /* DG_ENEMY_H_ */
