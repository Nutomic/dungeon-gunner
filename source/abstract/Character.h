/*
 * Actor.h
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#ifndef DG_ACTOR_H_
#define DG_ACTOR_H_

#include "Circle.h"

class Pathfinder;
class World;
class Weapon;
class Yaml;

/**
 * Provides think function for AI, manages health, drops body on death.
 */
class Character : public Circle {
public:
	explicit Character(const sf::Vector2f& position, Category category,
			unsigned short mask, const Yaml& config, World& world,
			Pathfinder& pathfinder);
	virtual ~Character() = 0;

	void onDamage(int damage);

public:
	/// Maximum distance where an enemy will be detected.
	static const float VISION_DISTANCE;

protected:
	virtual void onThink(int elapsed);
	virtual void onDeath();
	float getMovementSpeed() const;
	void pullTrigger();
	void releaseTrigger();
	bool setDestination(const sf::Vector2f& destination);
	bool isMoving() const;
	bool isVisible(const sf::Vector2f& target) const;
	std::vector<std::shared_ptr<Character> > getCharacters() const;

private:
	void move();

private:
	friend class World;
	World& mWorld;
	Pathfinder& mPathfinder;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
	const float mMovementSpeed;
	std::unique_ptr<Weapon> mWeapon;
	std::vector<sf::Vector2f> mPath; //< Contains nodes to reach a set destination.
	sf::Vector2f mLastPosition;
};

#endif /* DG_ACTOR_H_ */
