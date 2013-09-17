/*
 * Heal.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_HEAL_H_
#define DG_HEAL_H_

#include "Gadget.h"

#include "../../util/Yaml.h"

class Heal : public Gadget {
public:
	Heal();

protected:
	void onUse(Character& character) override;
	void onThink(int elapsed) override;
	GadgetType getType() const override;

private:
	Heal(const Yaml& config);

private:
	static const std::string CONFIG_NAME;
	const int mHealedTotal;
	const sf::Time mTimePerPoint;

	int mHealed;

	Character* mCharacter;
	thor::Timer mTimer;
};

#endif /* DG_HEAL_H_ */
