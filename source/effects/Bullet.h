/*
 * Bullet.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_BULLET_H_
#define DG_BULLET_H_

#include "../particle/Particle.h"

/**
 * Bullet particle fired by a weapon, may damage actors.
 */
class Bullet : public Particle {
// Public functions.
public:
	Bullet(const Vector2f& position, b2World& world, const std::shared_ptr<sf::Texture>& texture,
			Physical& shooter, float direction, int damage);

	void onCollide(Physical& other, uint16 category);
	bool doesCollide(Physical& other);

// Public variables.
public:
	static const Vector2i SIZE;
// Private variables.
private:
	static const float SPEED;
	Physical& mShooter;
	const int mDamage;
};

#endif /* DG_BULLET_H_ */
