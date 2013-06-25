/*
 * Weapon.h
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#ifndef DG_WEAPON_H_
#define DG_WEAPON_H_

#include <string>
#include <random>

#include <SFML/System.hpp>

#include <Thor/Time.hpp>

#include "../util/Yaml.h"

class Character;
class World;
class Particle;
class Yaml;

class Weapon {
public:
	explicit Weapon(World& world, Character& holder, const Yaml& config);

	void pullTrigger();
	void releaseTrigger();
	void onThink(int elapsed);
	int getMagazineAmmo() const;
	int getTotalAmmo() const;
	void reload();

private:
	void fire();
	void insertProjectile(float angle);

private:
	World& mWorld;
	Character& mHolder;

	thor::Timer mTimer;
	const Yaml mProjectile;
	const int mDamage;
	const float mProjectileSpeed;
	const int mFireInterval;
	const int mReloadTime;
	bool mFiring;
	const bool mAutomatic;
	const int mMagazineSize;
	int mMagazineAmmo;
	const int mMaxTotalAmmo;
	int mTotalAmmo;
	bool mIsReloading = false;
	const int mPellets;
	const float mPelletSpread;
	const bool mReloadSingle;
	const float mSpread;
	const float mSpreadMoving;
	std::default_random_engine mGenerator;

};

#endif /* DG_WEAPON_H_ */
