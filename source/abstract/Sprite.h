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
		Data(const sf::Vector2f& position, float angle,	Category category,
				unsigned short mask);
		const sf::Vector2f& position;
		float angle;
		Category category;
		unsigned short mask;
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
	Sprite(const Data& data, const Yaml& config);
	virtual ~Sprite() = 0;

	sf::Vector2f getPosition() const;
	sf::Vector2f getSpeed() const;
	float getAngle() const;
	bool getDelete() const;
	Category getCategory() const;
	sf::Vector2f getSize() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool collisionEnabled(Category category) const;

	virtual void onCollide(std::shared_ptr<Sprite> other);

// Public variables.
public:
	static const std::string KEY_SIZE;
	static const std::string KEY_RADIUS;
	static const std::string KEY_TEXTURE;

// Protected functions.
protected:
	void setDelete(bool value);
	void setSpeed(sf::Vector2f direction, float speed);
	void setAngle(float angle);
	void setPosition(const sf::Vector2f& position);
	float getRadius() const;

// Private types.
private:
	class Shape {
	public:
		enum class Type {
			CIRCLE,
			RECTANGLE
		};

		Type type;
		std::shared_ptr<sf::Shape> shape;
	};

// Private variables.
private:
	friend class World;

	Shape mShape;
	std::shared_ptr<sf::Texture> mTexture;
	sf::Vector2f mSpeed;
	Category mCategory;
	unsigned short mMask;
	bool mDelete;
};

#endif /* DG_SPRITE_H_ */
