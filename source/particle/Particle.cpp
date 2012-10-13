/*
 * Particle.cpp
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#include "Particle.h"

Particle::Particle(const Yaml& config, const PhysicalData& data) :
		Sprite(config, data) {
}

Particle::~Particle() {
}

