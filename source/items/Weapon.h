/*
 * Weapon.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_WEAPON_H_
#define DG_WEAPON_H_

#include <Thor/Particles.hpp>

#include "../abstract/Body.h"
#include "../particle/Emitter.h"
#include "../util/Timer.h"
#include "../util/Yaml.h"

class Emitter;
class Instances;
class Body;
class Timer;
class Yaml;

/**
 * Loading mechanism:
 * 	- pass enum value and load mapped xml
 * 	- pass xml filename
 */
class Weapon : public Emitter {
// Public functions.
public:
	Weapon(World& world, Body& holder, const Yaml& config);

	void fire();

// Protected functions.
protected:
	std::shared_ptr<Particle> createParticle();

// Private variables.
private:
	static const String KEY_BULLET;
	static const String DEFAULT_BULLET;
	static const String KEY_INTERVAL;
	static const int DEFAULT_INTERVAL;

	World& mWorld;
	Body& mHolder;

	Vector2f mOffset; //< Offset to the point where bullets are inserted (from holder center).
	const String mBullet;
	Timer mTimer;

};

#endif /* DG_WEAPON_H_ */
