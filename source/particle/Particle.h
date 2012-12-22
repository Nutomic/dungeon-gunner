/*
 * Particle.h
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#ifndef DG_PARTICLE_H_
#define DG_PARTICLE_H_

#include "../abstract/Sprite.h"
#include "../util/Yaml.h"

class Sprite;
class Yaml;

/**
 * Prototype for a particle.
 */
class Particle : public Sprite {
// Public functions.
public:
	Particle(const Yaml& config, const Data& data);
	virtual ~Particle();
};

#endif /* DG_PARTICLE_H_ */
