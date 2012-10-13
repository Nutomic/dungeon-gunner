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
 * This uses lazy initialization and should be thread safe in C++11 (untested).
 * To use, just make a subclass with only a private default constructor and Singleton<T>
 * as friend class.
 *
 * @code
 * class MyClass : public Singleton<MyClass> {
 * private:
 * 		friend class Singleton<MyClass>;
 * 		MyClass() = default;
 * };
 * @endcode
 */
template <class T>
class Singleton : public sf::NonCopyable {
// Public functions.
public:
	static T& i();
};

/**
* Returns the instance of T.
*/
template <class T>
T& Singleton<T>::i() {
	static T s;
	return s;
};

#endif /* DG_SINGLETON_H_ */
