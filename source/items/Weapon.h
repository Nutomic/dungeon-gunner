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

#include <Thor/Time.hpp>

#include "../particle/Emitter.h"
#include "../util/Yaml.h"

class Character;
class World;
class Particle;
class Yaml;

/**
 * Loading mechanism:
 * 	- pass enum value and load mapped xml
 * 	- pass xml filename
 */
class Weapon : public Emitter {
public:
	explicit Weapon(World& world, Character& holder, const Yaml& config);

	void pullTrigger();
	void releaseTrigger();
	void onThink(int elapsed);
	int getMagazineAmmo() const;
	int getTotalAmmo() const;
	void reload();

protected:
	std::shared_ptr<Sprite> createParticle();

private:
	Character& mHolder;

	thor::Timer mTimer;
	const Yaml mProjectile;
	const int mDamage;
	const float mProjectileSpeed;
	const int mFireInterval;
	const int mReloadTime;
	bool mFire;
	bool mAutomatic;
	const int mMagazineSize;
	int mMagazineAmmo;
	const int mMaxTotalAmmo;
	int mTotalAmmo;
	bool mIsReloading = false;

};

#endif /* DG_WEAPON_H_ */
