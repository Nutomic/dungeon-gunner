/*
 * Corpse.h
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#ifndef DG_CORPSE_H_
#define DG_CORPSE_H_

#include "../abstract/Sprite.h"
#include "../util/Yaml.h"

class Sprite;
class Yaml;

class Corpse : public Sprite {
// Public functions.
public:
	Corpse(b2World& world, const Vector2f& position, const Yaml& config);
};

#endif /* DG_CORPSE_H_ */
