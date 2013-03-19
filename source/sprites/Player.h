/*
 * Player.h
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#ifndef DG_PLAYER_H_
#define DG_PLAYER_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../abstract/Character.h"
#include "../items/Weapon.h"
#include "../util/Yaml.h"

class Character;
class Instances;
class Weapon;
class Yaml;

/**
 * Player object.
 */
class Player : public Character {
// Public types.
public:
	/**
	 * Movement directions that can be set via Player::setDirection().
	 */
	enum Direction : unsigned char {
		RIGHT = 1 << 0,
		LEFT = 1 << 1,
		UP = 1 << 2,
		DOWN = 1 << 3
	};

// Public functions.
public:
	Player(World& world, const sf::Vector2f& position, const Yaml& config);

	void setCrosshairPosition(const sf::Vector2f& position);
	void pullTrigger();
	void releaseTrigger();
	void move(const sf::Vector2f& destination);
	void setDirection(Direction direction, bool unset);

// Private functions.
private:
	void onThink(int elapsed);

// Private variables.
private:
	sf::Vector2f mCrosshairPosition; //< Relative position of the point to fire at (mouse cursor).
	unsigned char mDirection; //< Current movement direction for direct control.
};

#endif /* DG_PLAYER_H_ */
