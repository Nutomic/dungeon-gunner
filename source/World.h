/*
 * World.h
 *
 *  Created on: 29.08.2012
 *      Author: Felix
 */

#ifndef DG_WORLD_H_
#define DG_WORLD_H_

#include "abstract/Character.h"
#include "abstract/Sprite.h"

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
	void step(int elapsed);
	void think(int elapsed);
	std::vector<std::shared_ptr<Character> >
			getCharacters(const sf::Vector2f& position, float maxDistance) const;
	bool raycast(const sf::Vector2f& lineStart,
			const sf::Vector2f& lineEnd) const;

private:

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
   	void applyMovement(std::shared_ptr<Sprite> sprite, int elapsed);

private:
	std::map<Sprite::Category, std::vector<std::shared_ptr<Sprite> > > mDrawables;
	std::vector<std::shared_ptr<Character> > mCharacters;
};

#endif /* DG_WORLD_H_ */
