/*
 * Bullet.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_BULLET_H_
#define DG_BULLET_H_

#include "../particle/Particle.h"
#include "../util/String.h"
#include "../util/Yaml.h"

class Particle;
class Yaml;

/**
 * Bullet particle fired by a weapon, may damage actors.
 */
class Bullet : public Particle {
// Public functions.
public:
	Bullet(const Vector2f& position, b2World& world, Physical& shooter, float direction,
			const Yaml& config);

	void onCollide(Physical& other, uint16 category);
	bool doesCollide(Physical& other);

// Public variables.
public:
	static const Vector2i SIZE;

// Private variables.
private:
	static const String KEY_DAMAGE;
	static const String KEY_SPEED;

	Physical& mShooter;
	const int mDamage;
	const float mSpeed;
};

#endif /* DG_BULLET_H_ */
