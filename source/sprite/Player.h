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

#include "../Pathfinder.h"
#include "../abstract/Actor.h"
#include "../abstract/Sprite.h"
#include "../items/Weapon.h"
#include "../util/Vector.h"

class Actor;
class Pathfinder;
class Sprite;
class Weapon;

/**
 * Player object.
 */
class Player : public Sprite, public Actor {
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
	Player(b2World& world, Collection& collection, const Vector2f& position, Pathfinder& pathfinder);

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
	static const float SPEED;
	static const Vector2i SIZE;
	/// The distance to a point where it is considered reached.
	static const float POINT_REACHED_DISTANCE;

	Weapon mWeapon; //< Weapon object used for Player::fire().
	Vector2f mCrosshairPosition; //< Relative position of the point to fire at (mouse cursor).

	uint8 mDirection; //< Current movement direction for direct control.

	Pathfinder& mPathfinder;
	std::vector<Vector2f> mPath;
};

#endif /* DG_PLAYER_H_ */
