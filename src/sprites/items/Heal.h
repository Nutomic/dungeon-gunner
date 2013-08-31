/*
 * SlowHeal.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_SLOWHEAL_H_
#define DG_SLOWHEAL_H_

#include "Gadget.h"

class Heal : public Gadget {
public:
	Heal();

protected:
	void onUse(Character& character) override;
	void onThink(int elapsed) override;
	sf::Time getCooldownTime() const override;
	GadgetType getType() const override;

private:
	Character* mCharacter;
	thor::Timer mTimer;
	int mHealedTotal = 50;
};

#endif /* DG_SLOWHEAL_H_ */
