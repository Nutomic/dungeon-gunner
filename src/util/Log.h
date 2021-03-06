/*
 * Log.h
 *
 *  Created on: 25.07.2012
 *      Author: Felix
 */

#ifndef DG_LOG_H_
#define DG_LOG_H_

#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#ifdef RELEASE

#define LOG_E(str)
#define LOG_W(str)
#define LOG_D(str)
#define LOG_I(str)

#else

/**
 * Logging functions for different levels.
 *
 * @code
 * LOG_E("something bad happened");
 * LOG_I(1 << 2 << 3 << "takeoff");
 * @endcode
 */

/**
 * \def LOG_E(str)
 * Log an error to the error stream.
 */
#define LOG_E(str) std::cout << __FILE__ << ":" << __LINE__ << " " << "Error:   " << str << std::endl

/**
 * \def LOG_E(str)
 * Log a warning to the output stream.
 */
#define LOG_W(str) std::cout << __FILE__ << ":" << __LINE__ << " " << "Warning: " << str << std::endl

/**
 * \def LOG_E(str)
 * Log a debug message to the output stream.
 */
#define LOG_D(str) std::cout << __FILE__ << ":" << __LINE__ << " " << "Debug:   " << str << std::endl

/**
 * \def LOG_E(str)
 * Log an info to the output stream.
 */
#define LOG_I(str) std::cout << __FILE__ << ":" << __LINE__ << " " << "Info:    " << str << std::endl

/**
 * Adds an output operator specalization for sf::Vector2f.
 */
inline std::ostream&
operator<<(std::ostream& os, const sf::Vector2f& vector) {
    os << "(" << vector.x << ", " << vector.y << ")";
    return os;
}

/**
 * Adds an output operator specalization for sf::Vector2i
 */
inline std::ostream&
operator<<(std::ostream& os, const sf::Vector2i& vector) {
    os << "(" << vector.x << ", " << vector.y << ")";
    return os;
}

/**
 * Adds an output operator specalization for sf::FloatRect
 */
inline std::ostream&
operator<<(std::ostream& os, const sf::FloatRect& rect) {
    os << "(x: " << rect.left << ", y: " << rect.top << ", w: " <<
    		rect.width << ", h: " << rect.height << ")";
    return os;
}

/**
 * Adds an output operator specalization for sf::IntRect
 */
inline std::ostream&
operator<<(std::ostream& os, const sf::IntRect& rect) {
    os << "(x: " << rect.left << ", y: " << rect.top << ", w: " <<
    		rect.width << ", h: " << rect.height << ")";
    return os;
}

#endif

#endif /* DG_LOG_H_ */
