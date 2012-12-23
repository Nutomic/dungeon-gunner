/*
 * Emitter.h
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#ifndef DG_EMITTER_H_
#define DG_EMITTER_H_

#include "../abstract/Sprite.h"
#include "../World.h"
#include "Particle.h"

class Sprite;
class World;
class Particle;

class Emitter {
// Public functions.
public:
	Emitter(World& collection);
	virtual ~Emitter();

// Protected functions.
protected:
	void emit();
	/// Creates a particle. Allows to use a user-defined particle class and custom settings.
	virtual std::shared_ptr<Particle> createParticle() = 0;

// Private variables.
private:
	World& mCollection;
};

#endif /* DG_EMITTER_H_ */
