/*
 * HealthOrb.h
 *
 *  Created on: 02.09.2013
 *      Author: Felix
 */

#ifndef DG_HEALTHORB_H_
#define DG_HEALTHORB_H_

#include "Item.h"

#include "../../util/Yaml.h"

class HealthOrb : public Item {
public:
	HealthOrb();
	std::string getName() const;
	int getAmountHealed() const;

private:
	HealthOrb(const Yaml& config);

private:
	static const std::string CONFIG_NAME;
	const std::string mName;
	const int mAmountHealed;
};

#endif /* DG_HEALTHORB_H_ */
