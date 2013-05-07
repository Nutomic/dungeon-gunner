/*
 * Particle.h
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#ifndef DG_PARTICLE_H_
#define DG_PARTICLE_H_

#include "../abstract/Circle.h"

class Yaml;

/**
 * Prototype for a particle.
 */
class Particle : public Circle {
public:
	explicit Particle(const sf::Vector2f& position, Category category,
			unsigned short mask, const Yaml& config,
			const sf::Vector2f& direction);
	virtual ~Particle();
};

#endif /* DG_PARTICLE_H_ */
