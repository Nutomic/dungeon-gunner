/*
 * Timer.cpp
 *
 *  Created on: 19.10.2012
 *      Author: Felix
 */

#include "Timer.h"

#include <assert.h>

/**
 * Initializes time limit to zero.
 */
Timer::Timer() {
}

/**
 * Initializes time limit to parameter value.
 */
Timer::Timer(sf::Time timeLimit) :
		mLimit(timeLimit) {
}

/**
 * Returns true if the time set in setLimit() has passed since calling start().
 */
bool
Timer::isExpired() const {
	return mClock.getElapsedTime() >= mLimit;
}

/**
 * Starts the timer.
 */
void
Timer::start() {
	mClock.restart();
}

/**
 * Sets the time limit to run to.
 */
void
Timer::setLimit(sf::Time timeLimit) {
	assert(timeLimit > sf::Time::Zero);
	mLimit = timeLimit;
}
