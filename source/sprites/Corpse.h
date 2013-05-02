/*
 * Corpse.h
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#ifndef DG_CORPSE_H_
#define DG_CORPSE_H_

#include "../abstract/Sprite.h"

class Corpse : public Sprite {
public:
	explicit Corpse(const sf::Vector2f& position);

private:
	static const std::string CONFIG;
};

#endif /* DG_CORPSE_H_ */
