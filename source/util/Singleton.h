/*
 * Singleton.h
 *
 *  Created on: 04.07.2012
 *      Author: Felix
 */

#ifndef DG_SINGLETON_H_
#define DG_SINGLETON_H_

#include <SFML/System.hpp>

/**
 * Template class for inheriting singleton behaviour.
 *
 * To use, just make a subclass with only a private default constructor and Singleton<T>
 * as friend class.
 */
template <class T>
class Singleton : public sf::NonCopyable {
  public:
	/**
	 * Get the instance of this class.
	 */
	 static T& i() {
		static T s;
		return s;
	 };
};

#endif /* DG_SINGLETON_H_ */
