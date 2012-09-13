/*
 * Physical.h
 *
 *  Created on: 11.08.2012
 *      Author: Felix
 */

#ifndef DG_PHYSICAL_H_
#define DG_PHYSICAL_H_

#include <Box2D/Box2D.h>

#include "../util/Vector.h"

/**
 * An object with physical properties.
 *
 * @warning May only handle bodies with one fixture.
 */
class Physical {
// Public types.
public:
	/**
	 * POD container that carries all data required to construct this class.
	 */
	class PhysicalData {
	public:
		PhysicalData() = default;
		PhysicalData(const Vector2f& position, const Vector2i& size, b2World& world,
				uint16 category, uint16 maskExclude, bool moving, bool bullet = false,
				bool circle = false);
		/// World position of the body in pixel coordinates.
		const Vector2f& position;
		/// Pixel size of the body if it is a box.
		Vector2i size;
		/// Box2D world object.
		b2World& world;
		/// The category for collision filtering. Only one may be set. @link Physical::Category
		uint16 category;
		/// All categories set here will have collisions disabled with this object.
		uint16 maskExclude;
		/// True if the body may move on its own (player, monster).
		bool moving;
		/// True if the object is a bullet.
		bool bullet;
		/// True if the body collides as a circle. Radius is side length / 2, both sides must be equal.
		bool circle;
	};

	/**
	 * Categories of physical objects, for Box2D collision filtering.
	 *
	 * @warning An object may only have one category.
	 */
	enum Category {
		CATEGORY_NONSOLID = 0,
		CATEGORY_WORLD = 1 << 1,
		CATEGORY_ACTOR = 1 << 2,
		CATEGORY_PARTICLE = 1 << 3
	};

	/**
	 * Common Box2D collision masking values.
	 */
	enum Mask {
		MASK_NONE = 0xffff, //< Disables any collisions.
		MASK_ALL = 0 //< Enables all collisions.
	};

// Public functions.
public:
	Physical(const PhysicalData& data);
	virtual ~Physical() = 0;

	Vector2f getPosition() const;
	Vector2f getSpeed() const;
	float getAngle() const;
	bool getDelete() const;
	uint16 getCategory() const;

	virtual bool doesCollide(Physical& other);
	virtual void onCollide(Physical& other, uint16 category);

// Protected functions.
protected:
	void setDelete(bool value);
	void setSpeed(Vector2f direction, float speed);
	void setAngle(float angle);

// Protected variables.
protected:
	// Currently protected to allow for (debug only) direct player input.
	b2Body* mBody;

// Private variables.
private:
	bool mDelete;
};

#endif /* DG_PHYSICAL_H_ */
