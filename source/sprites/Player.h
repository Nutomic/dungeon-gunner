/*
 * Player.h
 *
 *  Created on: 21.07.2012
 *      Author: Felix
 */

#ifndef DG_PLAYER_H_
#define DG_PLAYER_H_

#include "../abstract/Character.h"

class Yaml;
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
		RIGHT = 1 << 0,
		LEFT = 1 << 1,
		UP = 1 << 2,
		DOWN = 1 << 3
	};

public:
	explicit Player(World& world, TileManager& tileManager,
			Pathfinder& pathfinder,	const sf::Vector2f& position,
			const Yaml& config);

	void setCrosshairPosition(const sf::Vector2f& position);
	void pullTrigger();
	void releaseTrigger();
	void setDirection(Direction direction, bool unset);
	void setDestination(const sf::Vector2f& destination);

private:
	void onThink(int elapsed);

private:
	sf::Vector2f mCrosshairPosition; //< Relative position of the point to fire at (mouse cursor).
	unsigned char mDirection; //< Current movement direction for direct control.
};

#endif /* DG_PLAYER_H_ */
