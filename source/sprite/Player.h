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

#include "../util/Instances.h"
#include "../Pathfinder.h"
#include "../abstract/Character.h"
#include "../items/Weapon.h"
#include "../util/Vector.h"
#include "../util/Yaml.h"

class Character;
class Instances;
class Pathfinder;
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
	enum class Direction : uint8 {
		RIGHT = 1 << 0,
		LEFT = 1 << 1,
		UP = 1 << 2,
		DOWN = 1 << 3
	};

// Public functions.
public:
	Player(const Instances& instances, const Vector2f& position, const Yaml& config);

	void setCrosshairPosition(const Vector2f& position);
	void fire();
	void move(const Vector2f& destination);
	void setDirection(Direction direction, bool unset);

// Private functions.
private:
	void onCollide(Physical& other, uint16 category);
	void onThink(float elapsedTime);

// Private variables.
private:
	Vector2f mCrosshairPosition; //< Relative position of the point to fire at (mouse cursor).
	uint8 mDirection; //< Current movement direction for direct control.
};

#endif /* DG_PLAYER_H_ */
