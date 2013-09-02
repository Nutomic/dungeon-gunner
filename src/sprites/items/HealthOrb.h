/*
 * HealthOrb.h
 *
 *  Created on: 02.09.2013
 *      Author: Felix
 */

#ifndef DG_HEALTHORB_H_
#define DG_HEALTHORB_H_

#include "Item.h"

class HealthOrb : public Item {
public:
	static const int AMOUNT_HEALED = 50;

public:
	HealthOrb();
	std::string getName() const;
};

#endif /* DG_HEALTHORB_H_ */
