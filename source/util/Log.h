/*
 * Log.h
 *
 *  Created on: 25.07.2012
 *      Author: Felix
 */

#ifndef DG_LOG_H_
#define DG_LOG_H_

#include <iostream>

#include "Vector.h"

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
 * Adds an output operator specalization for Vector2f
 */
inline std::ostream&
operator<<(std::ostream& os, const Vector2f& vector) {
    os << "(" << vector.x << ", " << vector.y << ")";
    return os;
}

#endif /* DG_LOG_H_ */
