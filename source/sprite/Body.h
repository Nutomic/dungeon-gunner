/*
 * Body.h
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#ifndef DG_BODY_H_
#define DG_BODY_H_

#include "../abstract/Sprite.h"
#include "../util/Yaml.h"

class Sprite;
class Yaml;

class Body : public Sprite {
// Public functions.
public:
	Body(b2World& world, const Vector2f& position, const Yaml& config);
};

#endif /* DG_BODY_H_ */
