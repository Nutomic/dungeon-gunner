/*
 * Weapon.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_WEAPON_H_
#define DG_WEAPON_H_

#include <Thor/Particles.hpp>

#include "../Instances.h"
#include "../abstract/Physical.h"
#include "../particle/Emitter.h"
#include "../util/Yaml.h"

class Emitter;
class Instances;
class Physical;

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
	static const String KEY_DAMAGE;

	Physical& mHolder;
	std::shared_ptr<sf::Texture> mBulletTexture;
	b2World& mWorld;

	const Vector2f mOffset; //< Offset to the point where bullets are inserted (from holder center).
	const int mDamage;
};

#endif /* DG_WEAPON_H_ */
