/*
 * Sprite.h
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#ifndef DG_SPRITE_H_
#define DG_SPRITE_H_

#include <memory>

#include <SFML/Graphics.hpp>

#include "../../util/Vector.h"

/**
 * An sprite that is rendered in the world.
 */
class Sprite : public sf::Drawable {
public:
	/**
	 * Categories of objects for filtering.
	 * The order of categories is also used for render order (higher number on top).
	 */
	enum Category {
		CATEGORY_WORLD = 1 << 0,
		CATEGORY_DECORATION = 1 << 1,
		CATEGORY_NONSOLID = 1 << 2,
		CATEGORY_PARTICLE = 1 << 3,
		CATEGORY_ACTOR = 1 << 4
	};

	/**
	 * Common collision masking values.
	 */
	enum Mask : unsigned short {
		MASK_ALL = 0xffff, //< Enables all collisions.
		MASK_NONE = 0 //< Disables any collisions.
	};

// Public functions.
public:
	explicit Sprite(const Vector2f& position, Category category,
			unsigned short mask, const Vector2f& size,
			const std::string& texture,	const Vector2f& direction);
	virtual ~Sprite() = default;

	Vector2f getPosition() const;
	Vector2f getSpeed() const;
	Vector2f getDirectionVector() const;
	float getDirection() const;
	bool getDelete() const;
	Category getCategory() const;
	Vector2f getSize() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool collisionEnabled(Category category) const;
	bool isInside(const sf::FloatRect& rect) const;

	virtual bool testCollision(std::shared_ptr<Sprite> other,
			Vector2f& offsetFirst, const Vector2f& offsetSecond) = 0;
	virtual void onCollide(std::shared_ptr<Sprite> other);

protected:
	void setDelete(bool value);
	void setSpeed(Vector2f direction, float speed);
	void setDirection(const Vector2f& direction);
	void setPosition(const Vector2f& position);
	void setTexture(const std::string& texture);

private:
	friend class CollisionModel;
	friend class World;

	sf::RectangleShape mShape;
	std::shared_ptr<sf::Texture> mTexture;
	Vector2f mSpeed;
	Category mCategory;
	unsigned short mMask;
	bool mDelete = false;
};

#endif /* DG_SPRITE_H_ */
