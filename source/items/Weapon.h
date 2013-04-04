/*
 * Weapon.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_WEAPON_H_
#define DG_WEAPON_H_

#include <string>

#include <SFML/System.hpp>

#include "../particle/Emitter.h"

class Sprite;
class Yaml;
class World;
class Particle;

/**
 * Loading mechanism:
 * 	- pass enum value and load mapped xml
 * 	- pass xml filename
 */
class Weapon : public Emitter {
public:
	explicit Weapon(World& world, Sprite& holder, const Yaml& config);

	void pullTrigger();
	void releaseTrigger();
	void onThink(int elapsed);

protected:
	std::shared_ptr<Sprite> createParticle();

private:
	static const std::string KEY_BULLET;
	static const std::string DEFAULT_BULLET;
	static const std::string KEY_INTERVAL;
	static const int DEFAULT_INTERVAL;
	static const std::string KEY_AUTOMATIC;
	static const bool DEFAULT_AUTOMATIC;

	Sprite& mHolder;

	sf::Vector2f mOffset; //< Offset to the point where bullets are inserted (from holder center).
	const std::string mBullet; //< Bullet config filename.
	int mLastShotWaitInterval; //< Remaining time left after firing last bullet before firing next one.
	const int mFireInterval; //< Time between firing bullets.
	bool mFire; //< True if the trigger is pulled.
	bool mAutomatic; //< True if the weapon continues firing after pulling the trigger once.

};

#endif /* DG_WEAPON_H_ */
