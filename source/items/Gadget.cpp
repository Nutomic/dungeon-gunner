/*
 * Gadget.cpp
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#include "Gadget.h"

#include "../abstract/Character.h"

void
Gadget::use(Character& character) {
	if (mCooldownTimer.isExpired()) {
		onUse(character);
		mCooldownTimer.restart(getCooldownTime());
	}
}
