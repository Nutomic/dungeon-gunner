/*
 * Player.h
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#ifndef DG_PLAYER_H_
#define DG_PLAYER_H_

#include "abstract/Character.h"

class World;

/**
 * Player object.
 */
class Player : public Character {
public:
	/**
	 * Movement directions that can be set via Player::setDirection().
	 */
	enum Direction : unsigned char {
		NONE = 0,
		RIGHT = 1 << 0,
		LEFT = 1 << 1,
		UP = 1 << 2,
		DOWN = 1 << 3
	};

public:
	explicit Player(World& world, Pathfinder& pathfinder,
			const Vector2f& position, const EquippedItems& items);

	Vector2f getCrosshairPosition() const;
	void setCrosshairPosition(const Vector2f& position);
	using Character::pullTrigger;
	using Character::releaseTrigger;
	void setDirection(Direction direction, bool unset);
	void setDestination(const Vector2f& destination);
	using Character::getWeaponName;
	using Character::reload;
	using Character::toggleWeapon;
	using Character::selectFirstWeapon;
	using Character::selectSecondWeapon;
	using Character::setLeftGadget;
	using Character::setRightGadget;
	using Character::useLeftGadget;
	using Character::useRightGadget;
	using Character::pickUpItem;

private:
	void onThink(int elapsed) override;

private:
	Vector2f mCrosshairPosition; //< Relative position of the point to fire at (mouse cursor).
	unsigned char mDirection = NONE; //< Current movement direction for direct control.
};

#endif /* DG_PLAYER_H_ */
