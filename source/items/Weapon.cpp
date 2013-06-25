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
		Emitter(world),
		mHolder(holder),
		mBullet(config.get("bullet", std::string("bullet.yaml"))),
		mFireInterval(config.get("fire_interval", 0)),
		mReloadTime(config.get("reload_time", 0)),
		mFire(false),
		mAutomatic(config.get("automatic", false)),
		mMagazineSize(config.get("magazine_size", 0)),
		mMagazineAmmo(mMagazineSize),
		mMaxTotalAmmo(config.get("max_total_ammo", 0)),
		mTotalAmmo(mMaxTotalAmmo) {
}

/**
 * Pull the trigger.
 */
void
Weapon::pullTrigger() {
	mFire = true;
}

/**
 * Release the trigger.
 */
void
Weapon::releaseTrigger() {
	mFire = false;
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
		mMagazineAmmo = (mTotalAmmo >= mMagazineSize)
				? mMagazineSize
				: mTotalAmmo;
		mTotalAmmo -= mMagazineAmmo;
		mIsReloading = false;
	}

	if (mFire && mMagazineAmmo != 0) {
		emit();
		if (!mAutomatic)
			mFire = false;
	}

	if (mMagazineAmmo == 0 && mTotalAmmo != 0)
		reload();
}

/**
 * Creates and fires a projectile.
 */
std::shared_ptr<Sprite>
Weapon::createParticle() {
	mTimer.restart(sf::milliseconds(mFireInterval));
	mMagazineAmmo--;

	// Minus to account for positive y-axis going downwards in SFML.
	sf::Vector2f offset(0, - mHolder.getRadius());
	thor::rotate(offset, thor::polarAngle(mHolder.getDirection()));
	return std::shared_ptr<Sprite>(new Bullet(mHolder.getPosition() + offset,
			mHolder, mHolder.getDirection(), Yaml(mBullet)));
}

int
Weapon::getMagazineAmmo() const {
	return mMagazineAmmo;
}

int
Weapon::getTotalAmmo() const {
	return mTotalAmmo;
}

void
Weapon::reload() {
	mIsReloading = true;
	mTimer.restart(sf::milliseconds(mReloadTime));
}
