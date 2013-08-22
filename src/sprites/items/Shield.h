/*
 * Shield.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_SHIELD_H_
#define DG_SHIELD_H_

#include "Gadget.h"

class RotatingShield;
class Sprite;

class Shield : public Gadget {
public:
	Shield();

protected:
	void onUse(Character& character) override;
	void onThink(int elapsed) override;
	sf::Time getCooldownTime() override;

private:
	Character* mCharacter;
	std::shared_ptr<RotatingShield> mRotatingShield;
};

#endif /* DG_SHIELD_H_ */
