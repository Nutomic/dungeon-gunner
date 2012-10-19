/*
 * Timer.h
 *
 *  Created on: 19.10.2012
 *      Author: Felix
 */

#ifndef DG_TIMER_H_
#define DG_TIMER_H_

#include <SFML/System.hpp>

/**
 * A small timer utility that allows testing whether a certain amount of time is over.
 *
 * Replacement for thor::Timer as it causes crashes.
 */
class Timer {
	// Public functions.
	public:
		Timer();
		Timer(sf::Time timeLimit);

		void start();
		void setLimit(sf::Time timeLimit);
		bool isExpired() const;
		
	// Private variables.
	private:
		sf::Clock mClock;
		sf::Time mLimit;
};

#endif /* DG_TIMER_H_ */
