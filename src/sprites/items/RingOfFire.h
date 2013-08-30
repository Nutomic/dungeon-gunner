/*
 * RingOfFire.h
 *
 *  Created on: 29.08.2013
 *      Author: Felix
 */

#ifndef DG_RINGOFFIRE_H_
#define DG_RINGOFFIRE_H_

#include "Gadget.h"
#include "../abstract/Character.h"
#include "../../util/Yaml.h"

/**
 * Gadget that fires bullets in all directions for multiple waves.
 */
class RingOfFire : public Gadget {
public:
	RingOfFire(World& world);

protected:
	void onUse(Character& character) override;
	void onThink(int elapsed) override;
	sf::Time getCooldownTime() override;

private:
	static const int WAVES_PER_USE = 3;
	int mCurrentWave = WAVES_PER_USE + 1;

	Character* mCharacter;
	thor::Timer mTimer;
	World& mWorld;
	const Yaml mBullet;
};

#endif /* DG_RINGOFFIRE_H_ */
