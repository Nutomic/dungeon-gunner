/*
 * pathfinder.h
 *
 *  Created on: 24.09.2012
 *      Author: Felix
 */

#ifndef PATHFINDER_H_
#define PATHFINDER_H_

#include <SFML/System.hpp>

#include "../abstract/Sprite.h"

class Sprite;

class Pathfinder {
// Public functions.
public:
	Pathfinder();

	std::vector<sf::Vector2f> getPath(Sprite& physical, const sf::Vector2f& destination);
};

#endif /* PATHFINDER_H_ */
