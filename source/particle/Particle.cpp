/*
 * Particle.cpp
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#include "Particle.h"

Particle::Particle(const sf::Vector2f& position, Category category,
		unsigned short mask, const Yaml& config,
		const sf::Vector2f& direction) :
	Circle(position, category, mask, config, direction) {
}

Particle::~Particle() {
}

