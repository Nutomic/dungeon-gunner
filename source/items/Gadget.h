/*
 * Gadget.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_GADGET_H_
#define DG_GADGET_H_

#include <Thor/Time.hpp>

class Character;

class Gadget {
public:
	virtual void use(Character& character);
	virtual void onThink(int elapsed) = 0;

protected:
	virtual void onUse(Character& character) = 0;
	virtual sf::Time getCooldownTime() = 0;

protected:
	thor::Timer mCooldownTimer;
};

#endif /* DG_GADGET_H_ */
