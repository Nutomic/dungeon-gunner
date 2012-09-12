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

#include "../abstract/Actor.h"
#include "../abstract/Sprite.h"
#include "../items/Weapon.h"
#include "../util/Vector.h"

class Sprite;

/**
 * Player object.
 */
class Player : public Sprite, public Actor {
// Public functions.
public:
	Player(b2World& world, Collection& collection, const Vector2f& position);

	void setCrosshairPosition(const Vector2f& position);
	void fire();
	void move(const Vector2f& destination);

// Protected functions.
protected:
	void onCollide(Physical& other, uint16 category);
	void onThink(float elapsedTime);

// Private variables.
private:
	static const float SPEED;
	static const Vector2i SIZE;

	Weapon mWeapon; //< Weapon object used for Player::fire().
	Vector2f mDestination; //< Absolute position of the movement destination.
	Vector2f mCrosshairPosition; //< Relative position of the point to fire at (mouse cursor).
};

#endif /* DG_PLAYER_H_ */
