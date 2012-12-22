/*
 * Body.h
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#ifndef DG_BODY_H_
#define DG_BODY_H_

#include "../types/String.h"
#include "../types/Vector.h"
#include "../util/Yaml.h"

class Yaml;

/**
 * An object with physical properties.
 *
 * @warning May only handle bodies with one fixture.
 */
class Body {
// Public types.
public:
	/**
	 * Categories of physical objects, for Box2D collision filtering.
	 * The order of categories is also used for render order in Collection::draw()
	 * (higher number on top).
	 *
	 * @warning An object may only have one category.
	 */
	enum Category {
		CATEGORY_WORLD = 1 << 1,
		CATEGORY_NONSOLID = 1 << 2,
		CATEGORY_PARTICLE = 1 << 3,
		CATEGORY_ACTOR = 1 << 4
	};

	/**
	 * POD container that carries all data required to construct an object of this class.
	 */
	class Data {
	public:
		Data() = default;
		Data(const Vector2f& position, float angle,	Category category, unsigned short maskExclude);
		const Vector2f& position;
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
	Body(const Data& data, const Yaml& config, const Vector2i& pSize = Vector2i());
	virtual ~Body() = 0;

	Vector2f getPosition() const;
	Vector2f getSpeed() const;
	float getAngle() const;
	bool getDelete() const;
	Category getCategory() const;
	Vector2i getSize() const;

	virtual bool doesCollide(Body& other);
	virtual void onCollide(Body& other, Category category);

// Public variables.
public:
	static const String KEY_SIZE;
	static const Vector2i DEFAULT_SIZE;

// Protected functions.
protected:
	void setDelete(bool value);
	void setSpeed(Vector2f direction, float speed);
	void setAngle(float angle);

// Private variables.
private:
	Vector2f mPosition;
	Vector2i mSize;
	Vector2f mSpeed;
	float mAngle;
	Category mCategory;
	unsigned short mMask;
	bool mDelete;
};

#endif /* DG_BODY_H_ */
