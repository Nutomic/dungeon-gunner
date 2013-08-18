/*
 * Weapon.cpp
 *
 *  Created on: 12.08.2012
 *      Author: Felix
 */

#include "Weapon.h"

#include <Thor/Vectors.hpp>

#include "../World.h"
#include "../effects/Bullet.h"
#include "../util/Yaml.h"

Weapon::Weapon(World& world, Character& holder, const Yaml& config) :
		Item(Vector2f(32, 32), "item.png"),
		mWorld(world),
		mHolder(&holder),
		mName(config.get("name", std::string())),
		mProjectile(config.get("bullet", std::string("bullet.yaml"))),
		mDamage(config.get("damage", 0)),
		mProjectileSpeed(config.get("projectile_speed", 0.0f)),
		mFireInterval(config.get("fire_interval", 0)),
		mReloadTime(config.get("reload_time", 0)),
		mFiring(false),
		mAutomatic(config.get("automatic", false)),
		mMagazineSize(config.get("magazine_size", 0)),
		mMagazineAmmo(mMagazineSize),
		mMaxTotalAmmo(config.get("max_total_ammo", 0)),
		mTotalAmmo(mMaxTotalAmmo),
		mPellets(config.get("pellets", 0)),
		mPelletSpread(config.get("pellet_spread", 0.0f)),
		mReloadSingle(config.get("reload_single", false)),
		mSpread(config.get("spread", 0.0f)),
		mSpreadMoving(config.get("spread_moving", 0.0f)),
		mMaxRange(config.get("max_range", 0.0f)),
		mRequiresAmmo(!config.get("requires_no_ammo", false)) {
}

/**
 * Pull the trigger.
 */
void
Weapon::pullTrigger() {
	mFiring = true;
}

/**
 * Release the trigger.
 */
void
Weapon::releaseTrigger() {
	mFiring = false;
}

/**
 * Fire if the trigger has been pulled, time between bullets is over, has ammo etc.
 *
 * @param elapsed Amount of time to simulate.
 */
void
Weapon::onThink(int elapsed) {
	if (!mTimer.isExpired())
		return;

	if (mIsReloading) {
		if (!mReloadSingle) {
			mMagazineAmmo = (mTotalAmmo >= mMagazineSize)
					? mMagazineSize
					: mTotalAmmo;
			mTotalAmmo -= mMagazineAmmo;
			mIsReloading = false;
		}
		else if (mTotalAmmo > 0) {
			mMagazineAmmo++;
			mTotalAmmo--;
			if (mMagazineAmmo == mMagazineSize)
				mIsReloading = false;
			else
				reload();
		}
		else
			mIsReloading = false;
	}

	if (mFiring && (!mRequiresAmmo || mMagazineAmmo != 0)) {
		fire();
		if (!mAutomatic)
			mFiring = false;
	}

	if (mRequiresAmmo && mMagazineAmmo == 0 && mTotalAmmo != 0)
		reload();
}

/**
 * Creates and fires a projectile.
 */
void
Weapon::fire() {
	mTimer.restart(sf::milliseconds(mFireInterval));
	if (mRequiresAmmo)
		mMagazineAmmo--;


	if (mPellets == 0)
		insertProjectile(0.0f);
	else
		for (int i = - mPellets / 2; i < mPellets / 2; i++) {
			insertProjectile(i * mPelletSpread);
		}
}

int
Weapon::getMagazineAmmo() const {
	return mMagazineAmmo;
}

int
Weapon::getTotalAmmo() const {
	return mTotalAmmo;
}

std::string
Weapon::getName() const {
	return mName;
}

void
Weapon::reload() {
	if (mMagazineAmmo == mMagazineSize)
		return;
	mIsReloading = true;
	mTimer.restart(sf::milliseconds(mReloadTime));
}

void
Weapon::cancelReload() {
	mIsReloading = false;
	// To make sure time isn't skipped.
	mTimer.restart(sf::milliseconds(mFireInterval));
}

void
Weapon::setHolder(Character& holder) {
    mHolder = &holder;
}

/**
 * Creates a new projectile and inserts it into the world.
 *
 * @param angle Inaccuracy of the projectile, 0 is straight forward.
 */
void
Weapon::insertProjectile(float angle) {
	Vector2f offset(mHolder->getDirection() * mHolder->getRadius());

	float spread = (mHolder->getSpeed() == Vector2f())
			? mSpread
			: mSpreadMoving;
	std::uniform_real_distribution<float> distribution(- spread, spread);
	angle += distribution(mGenerator) + 90.0f;

	Vector2f direction(thor::rotatedVector(mHolder->getDirection(), angle));

	std::shared_ptr<Sprite> projectile(new Bullet(mHolder->getPosition() + offset,
			*mHolder, direction, mProjectile, mProjectileSpeed,
			mDamage, mMaxRange));
	mWorld.insert(projectile);
}
