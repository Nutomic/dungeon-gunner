/*
 * Emitter.h
 *
 *  Created on: 15.08.2012
 *      Author: Felix
 */

#ifndef DG_EMITTER_H_
#define DG_EMITTER_H_

#include <memory>

class World;
class Particle;
class Sprite;

class Emitter {
public:
	explicit Emitter(World& world);
	virtual ~Emitter();

protected:
	void emit();
	/// Creates a particle. Allows to use a user-defined particle class and custom settings.
	virtual std::shared_ptr<Sprite> createParticle() = 0;

private:
	World& mWorld;
};

#endif /* DG_EMITTER_H_ */
