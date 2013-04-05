/*
 * Actor.h
 *
 *  Created on: 02.09.2012
 *      Author: Felix
 */

#ifndef DG_ACTOR_H_
#define DG_ACTOR_H_

#include "Sprite.h"

class TileManager;
class World;
class Weapon;
class Yaml;

/**
 * Provides think function for AI, manages health, drops body on death.
 */
class Character : public Sprite {
public:
	explicit Character(World& world, TileManager& tileManager,
			const Data& data, const Yaml& config);
	virtual ~Character() = 0;

	void onDamage(int damage);

protected:
	virtual void onThink(int elapsed);
	virtual void onDeath();
	float getMovementSpeed() const;
	void pullTrigger();
	void releaseTrigger();
	bool setDestination(const sf::Vector2f& destination);
	void move();
	bool isMoving() const;
	bool isVisible(const sf::Vector2f& target) const;
	std::vector<std::shared_ptr<Character> > getCharacters() const;

private:
	static const std::string KEY_HEALTH;
	static const int DEFAULT_HEALTH;
	static const std::string KEY_SPEED;
	static const float DEFAULT_SPEED;
	static const std::string KEY_WEAPON;
	static const std::string DEFAULT_WEAPON;
	/// The distance to a point where it is considered reached (in pixels).
	static const float POINT_REACHED_DISTANCE;
	/// Maximum distance where an enemy will be detected.
	static const float VISION_DISTANCE;

	friend class World;
	World& mWorld;
	TileManager& mTileManager;

	const int mMaxHealth;
	int mCurrentHealth; //< Current health. Between 0 and mMaxHealth.
	const float mMovementSpeed;
	std::unique_ptr<Weapon> mWeapon;
	std::vector<sf::Vector2f> mPath; //< Contains nodes to reach a set destination.
};

#endif /* DG_ACTOR_H_ */
