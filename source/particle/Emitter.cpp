/*
 * Emitter.cpp
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#include "Emitter.h"

Emitter::Emitter(Collection& collection) :
		mCollection(collection) {
}

Emitter::~Emitter() {
}

/**
 * Inserts a new particle into the system, using createParticle().
 */
void
Emitter::emit() {
	mCollection.insert(createParticle());
}
