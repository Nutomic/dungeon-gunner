/*
 * Particle.cpp
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#include "Particle.h"

Particle::Particle(const std::shared_ptr<sf::Texture>& texture, const PhysicalData& data,
	const Yaml& config) :
		Sprite(texture, data) {
}

Particle::~Particle() {
}

