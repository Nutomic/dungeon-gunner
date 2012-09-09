/*
 * String.h
 *
 *  Created on: 19.07.2012
 *      Author: Felix
 */
 
/**
 * Use this as a replacement for std::to_string as MingW does not support it.
 */

#ifndef DG_STRING_H_
#define DG_STRING_H_

#include <math.h>
#include <sstream>

#include <SFML/System.hpp>

/**
 * Converts any value to a string.
 *
 * @param val Any variable.
 * @return val converted to string.
 */
template <typename T>
sf::String
str(T val) {
	std::stringstream out;
	out << val;
	return out.str();
}

/**
 * Converts floating point variable to string,
 *
 * @param val Any floating point variable.
 * @param digits Number of decimal places to round to.
 * @return val converted to string.
 */
template <typename T>
sf::String
str(T val, int digits) {
	std::stringstream out;
	out.precision(digits);
	out << val;
	return out.str();
}

#endif /* DG_STRING_H_ */
