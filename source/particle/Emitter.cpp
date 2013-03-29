/*
 * Emitter.cpp
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#include "Emitter.h"

#include "../World.h"

Emitter::Emitter(World& world) :
		mWorld(world) {
}

Emitter::~Emitter() {
}

/**
 * Inserts a new particle into the system, using createParticle().
 */
void
Emitter::emit() {
	mWorld.insert(createParticle());
}
