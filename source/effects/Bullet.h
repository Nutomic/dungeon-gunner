/*
 * Bullet.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_BULLET_H_
#define DG_BULLET_H_

#include "../particle/Particle.h"

class Yaml;

/**
 * Bullet particle fired by a weapon, may damage actors.
 */
class Bullet : public Particle {
// Public functions.
public:
	Bullet(const sf::Vector2f& position, Sprite& shooter,
			sf::Vector2f direction, const Yaml& config);

	void onCollide(std::shared_ptr<Sprite> other);

// Private variables.
private:
	static const std::string KEY_DAMAGE;
	static const int DEFAULT_DAMAGE;
	static const std::string KEY_SPEED;
	static const float DEFAULT_SPEED;

	Sprite& mShooter;
	const int mDamage;
	const float mSpeed;
};

#endif /* DG_BULLET_H_ */
