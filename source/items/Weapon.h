/*
 * Weapon.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_WEAPON_H_
#define DG_WEAPON_H_

#include <Thor/Particles.hpp>

#include "../abstract/Sprite.h"
#include "../particle/Emitter.h"
#include "../util/Timer.h"
#include "../util/Yaml.h"

class Emitter;
class Instances;
class Sprite;
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
	Weapon(World& world, Sprite& holder, const Yaml& config);

	void pullTrigger();
	void releaseTrigger();
	void onThink(int elapsed);

// Protected functions.
protected:
	std::shared_ptr<Particle> createParticle();

// Private variables.
private:
	static const std::string KEY_BULLET;
	static const std::string DEFAULT_BULLET;
	static const std::string KEY_INTERVAL;
	static const int DEFAULT_INTERVAL;
	static const std::string KEY_AUTOMATIC;
	static const bool DEFAULT_AUTOMATIC;

	World& mWorld;
	Sprite& mHolder;

	sf::Vector2f mOffset; //< Offset to the point where bullets are inserted (from holder center).
	const std::string mBullet; //< Bullet config filename.
	int mLastShotWaitInterval; //< Remaining time left after firing last bullet before firing next one.
	const int mFireInterval; //< Time between firing bullets.
	bool mFire; //< True if the trigger is pulled.
	bool mAutomatic; //< True if the weapon continues firing after pulling the trigger once.

};

#endif /* DG_WEAPON_H_ */
