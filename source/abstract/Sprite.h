/*
 * Sprite.h
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#ifndef DG_SPRITE_H_
#define DG_SPRITE_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../util/Yaml.h"

class Yaml;

/**
 * An sprite that is rendered in the world.
 */
class Sprite : public sf::Drawable {
// Public types.
public:
	/**
	 * Categories of objects for filtering.
	 * The order of categories is also used for render order (higher number on top).
	 */
	enum Category {
		CATEGORY_WORLD = 1 << 1,
		CATEGORY_NONSOLID = 1 << 2,
		CATEGORY_PARTICLE = 1 << 3,
		CATEGORY_ACTOR = 1 << 4
	};

	/**
	 * Container that carries all data required to construct an object of this class.
	 */
	class Data {
	public:
		Data(const sf::Vector2f& position, float angle,	Category category, unsigned short maskExclude);
		const sf::Vector2f& position;
		float angle;
		Category category;
		unsigned short mask;
	};

	/**
	 * Common collision masking values.
	 */
	enum Mask : unsigned short {
		MASK_NONE = 0xffff, //< Disables any collisions.
		MASK_ALL = 0 //< Enables all collisions.
	};

// Public functions.
public:
	Sprite(const Data& data, const Yaml& config);
	virtual ~Sprite() = 0;

	sf::Vector2f getPosition() const;
	sf::Vector2f getSpeed() const;
	float getAngle() const;
	bool getDelete() const;
	Category getCategory() const;
	sf::Vector2f getSize() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool doesCollide(Sprite& other);
	virtual void onCollide(Sprite& other);

// Public variables.
public:
	static const std::string KEY_SIZE;
	static const std::string KEY_RADIUS;
	static const std::string KEY_TEXTURE;

// Protected functions.
protected:
	friend class World;

	void setDelete(bool value);
	void setSpeed(sf::Vector2f direction, float speed);
	void setAngle(float angle);
	void setPosition(const sf::Vector2f& position);

// Private variables.
private:
	std::unique_ptr<sf::Shape> mShape;
	std::shared_ptr<sf::Texture> mTexture;
	sf::Vector2f mSpeed;
	Category mCategory;
	unsigned short mMask;
	bool mDelete;
};

#endif /* DG_SPRITE_H_ */
