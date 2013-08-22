/*
 * World.h
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#ifndef DG_WORLD_H_
#define DG_WORLD_H_

#include "sprites/abstract/Character.h"
#include "sprites/abstract/Sprite.h"

class Character;
class Sprite;

/**
 * A collection of sprites, which can be put into different layers.
 *
 * Uses Sprite instead of sf::Drawable to also manage deleting objects.
 * Render order is determined by Physical::Category (higher number on top).
 */
class World : public sf::Drawable {
public:
	void insert(std::shared_ptr<Sprite> drawable);
	void insertCharacter(std::shared_ptr<Character> character);
	void remove(std::shared_ptr<Sprite> drawable);
	void step(int elapsed);
	void think(int elapsed);
	std::vector<std::shared_ptr<Character> >
			getCharacters(const Vector2f& position, float maxDistance) const;
	bool raycast(const Vector2f& lineStart,
			const Vector2f& lineEnd) const;
	std::vector<std::shared_ptr<Sprite> > getNearbySprites(
			const Vector2f& position, float radius) const;
	std::shared_ptr<Item> getClosestItem(const Vector2f& position) const;

private:

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
   	void applyMovement(std::shared_ptr<Sprite> sprite, int elapsed);

private:
	std::map<Sprite::Category, std::vector<std::shared_ptr<Sprite> > > mDrawables;
	std::vector<std::shared_ptr<Character> > mCharacters;
};

#endif /* DG_WORLD_H_ */
