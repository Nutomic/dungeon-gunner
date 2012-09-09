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
public:
	Weapon(Physical& holder, Collection& collection, b2World& world);
	~Weapon();

	void fire();

protected:
	std::shared_ptr<Particle> createParticle();

private:
	Physical& mHolder;
	std::shared_ptr<sf::Texture> mBulletTexture;
	b2World& mWorld;
};

#endif /* DG_WEAPON_H_ */
