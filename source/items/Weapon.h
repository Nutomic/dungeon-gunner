/*
 * Weapon.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_WEAPON_H_
#define DG_WEAPON_H_

#include <Thor/Particles.hpp>

#include "../abstract/Physical.h"
#include "../particle/Emitter.h"
#include "../types/Instances.h"
#include "../util/Yaml.h"

class Emitter;
class Instances;
class Physical;
class Yaml;

/**
 * Loading mechanism:
 * 	- pass enum value and load mapped xml
 * 	- pass xml filename
 */
class Weapon : public Emitter {
// Public functions.
public:
	Weapon(const Instances& instances, Physical& holder, const Yaml& config);

	void fire();

// Protected functions.
protected:
	std::shared_ptr<Particle> createParticle();

// Private variables.
private:
	static const String KEY_BULLET;

	Physical& mHolder;
	b2World& mWorld;

	Vector2f mOffset; //< Offset to the point where bullets are inserted (from holder center).
	const String mBullet;
};

#endif /* DG_WEAPON_H_ */
