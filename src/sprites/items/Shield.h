/*
 * Shield.h
 *
 *  Created on: 06.07.2013
 *      Author: Felix
 */

#ifndef DG_SHIELD_H_
#define DG_SHIELD_H_

#include "Gadget.h"

#include "../../util/Yaml.h"

class RotatingShield;
class Sprite;

class Shield : public Gadget {
public:
	Shield();

protected:
	void onUse(Character& character) override;
	void onThink(int elapsed) override;
	GadgetType getType() const override;

private:
	Shield(const Yaml& config);

private:
	static const std::string CONFIG_NAME;
	Character* mCharacter;
	std::shared_ptr<RotatingShield> mRotatingShield;
};

#endif /* DG_SHIELD_H_ */
