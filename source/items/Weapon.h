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

/**
 * Loading mechanism:
 * 	- pass enum value and load mapped xml
 * 	- pass xml filename
 */
class Weapon : public Emitter {
// Public functions.
public:
	Weapon(Physical& holder, Collection& collection, b2World& world, const Vector2i& holderSize);
	~Weapon();

	void fire();

// Protected functions.
protected:
	std::shared_ptr<Particle> createParticle();

// Private variables.
private:
	static const int BULLET_DAMAGE;

	Physical& mHolder;
	std::shared_ptr<sf::Texture> mBulletTexture;
	b2World& mWorld;
	const Vector2f mOffset; //< Offset to the point where bullets are inserted (from holder center).
};

#endif /* DG_WEAPON_H_ */
