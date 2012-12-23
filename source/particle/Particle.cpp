/*
 * Particle.cpp
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#include "Particle.h"

Particle::Particle(const Yaml& config, const Data& data) :
		Sprite(data, config) {
}

Particle::~Particle() {
}

