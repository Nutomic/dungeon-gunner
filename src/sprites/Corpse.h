/*
 * Corpse.h
 *
 *  Created on: 13.09.2012
 *      Author: Felix
 */

#ifndef DG_CORPSE_H_
#define DG_CORPSE_H_

#include "../abstract/Circle.h"

class Corpse : public Circle {
public:
	explicit Corpse(const Vector2f& position);
};

#endif /* DG_CORPSE_H_ */
