/*
 * Gadget.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_GADGET_H_
#define DG_GADGET_H_

#include <Thor/Time.hpp>

#include "Item.h"

class Character;

class Gadget : public Item {
public:
	/**
	 * Gadgets, ordered by strength.
	 */
	enum GadgetType {
		NONE,
		SHIELD,
		HEAL,
		RINGOFFIRE,
		_LAST
	};

public:
	Gadget(std::string name, int cooldown);
	static std::shared_ptr<Gadget> getGadget(World& world, GadgetType type);
	void use(Character& character);
	virtual void onThink(int elapsed) = 0;
	std::string getName() const;
	virtual GadgetType getType() const = 0;

protected:
	virtual void onUse(Character& character) = 0;

protected:
	thor::Timer mCooldownTimer;

private:
	const std::string mName;
	const sf::Time mCooldown;
};

#endif /* DG_GADGET_H_ */
