/*
 * Bullet.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_BULLET_H_
#define DG_BULLET_H_

#include "../abstract/Circle.h"

class Character;
class Yaml;

/**
 * Bullet particle fired by a weapon, may damage actors.
 */
class Bullet : public Circle {
public:
	explicit Bullet(const sf::Vector2f& position, Character& shooter,
			sf::Vector2f direction, const Yaml& config,	float speed,
			float damage);

	void onCollide(std::shared_ptr<Sprite> other);

private:
	const Character& mShooter;
	const int mDamage;
	const float mSpeed;
};

#endif /* DG_BULLET_H_ */
