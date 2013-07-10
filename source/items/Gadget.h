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
	Gadget(std::string name);
	void use(Character& character);
	virtual void onThink(int elapsed) = 0;
	std::string getName() const;

protected:
	virtual void onUse(Character& character) = 0;
	virtual sf::Time getCooldownTime() = 0;

protected:
	thor::Timer mCooldownTimer;

private:
	std::string mName;
};

#endif /* DG_GADGET_H_ */
