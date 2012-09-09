/*
 * Log.h
 *
 *  Created on: 25.07.2012
 *      Author: Felix
 */

#ifndef DG_LOG_H_
#define DG_LOG_H_

#include <iostream>

/**
 * \def LOG_E(str)
 * Log an error to the error stream.
 */
#define LOG_E(str) std::cerr << "Error:   " << __FILE__ << ":" << __LINE__ << " \"" << str << "\"" << std::endl

/**
 * \def LOG_E(str)
 * Log a warning to the output stream.
 */
#define LOG_W(str) std::cout << "Warning: " << __FILE__ << ":" << __LINE__ << " \"" << str << "\"" << std::endl

/**
 * \def LOG_E(str)
 * Log a debug message to the output stream.
 */
#define LOG_D(str) std::cout << "Debug:   " << __FILE__ << ":" << __LINE__ << " \"" << str << "\"" << std::endl

/**
 * \def LOG_E(str)
 * Log an info to the output stream.
 */
#define LOG_I(str) std::cout << "Info:    " << __FILE__ << ":" << __LINE__ << " \"" << str << "\"" << std::endl

#endif /* DG_LOG_H_ */