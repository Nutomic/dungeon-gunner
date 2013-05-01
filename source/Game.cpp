/*
 * Game.cpp
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#include "Game.h"

#include "generator/Generator.h"
#include "sprites/Enemy.h"
#include "sprites/Player.h"
#include "util/Yaml.h"

const int Game::FPS_GOAL = 60;

/**
 * Initializes game, including window and objects (sprites).
 */
Game::Game(sf::RenderWindow& window) :
		mWindow(window),
		mView(sf::Vector2f(0, 0), mWindow.getView().getSize()),
		mGenerator(mWorld, mPathfinder),
		mQuit(false),
		mPaused(false) {
	mWindow.setFramerateLimit(FPS_GOAL);
	mWindow.setKeyRepeatEnabled(true);

	mGenerator.generateTiles(sf::IntRect(-32, -32, 64, 64));
	mPlayer = std::shared_ptr<Player>(new Player(mWorld, mPathfinder,
			mGenerator.getPlayerSpawn(), Yaml("player.yaml")));
	mWorld.insertCharacter(mPlayer);
}

/**
 * Closes window.
 */
Game::~Game() {
	mWindow.close();
}

/**
 * Runs the game loop.
 */
void
Game::loop() {
	while (!mQuit) {
		input();

		int elapsed = mClock.restart().asMilliseconds();
		if (mPaused)
			elapsed = 0;

		mWorld.think(elapsed);

		mWorld.step(elapsed);
		render();
	}
}

/**
 * Handles general game input.
 */
void
Game::input() {
	sf::Event event;
    while (mWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			mQuit = true;
			break;
		case sf::Event::KeyPressed:
			keyDown(event);
			break;
		case sf::Event::KeyReleased:
			keyUp(event);
			break;
		case sf::Event::MouseButtonPressed:
			mouseDown(event);
			break;
		case sf::Event::MouseButtonReleased:
			mouseUp(event);
			break;
		case sf::Event::MouseMoved:
			mPlayer->setCrosshairPosition(convertCoordinates(event.mouseMove.x,
					event.mouseMove.y));
			break;
		default:
			break;
		}
    }
}

/**
 * Handles key up event. This is used for events that only fire once per keypress.
 */
void
Game::keyUp(const sf::Event& event) {
	switch (event.key.code) {
	case sf::Keyboard::Escape:
		mQuit = true;
		break;
	case sf::Keyboard::Space:
		mPaused = !mPaused;
		break;
	case sf::Keyboard::W:
		mPlayer->setDirection(Player::Direction::UP, true);
		break;
	case sf::Keyboard::S:
		mPlayer->setDirection(Player::Direction::DOWN, true);
		break;
	case sf::Keyboard::A:
		mPlayer->setDirection(Player::Direction::LEFT, true);
		break;
	case sf::Keyboard::D:
		mPlayer->setDirection(Player::Direction::RIGHT, true);
		break;
	default:
		break;
	}
}

/**
 * Handles key down event. This is used for any events that refire automatically.
 */
void
Game::keyDown(const sf::Event& event) {
	switch (event.key.code) {
	case sf::Keyboard::W:
		mPlayer->setDirection(Player::Direction::UP, false);
		break;
	case sf::Keyboard::S:
		mPlayer->setDirection(Player::Direction::DOWN, false);
		break;
	case sf::Keyboard::A:
		mPlayer->setDirection(Player::Direction::LEFT, false);
		break;
	case sf::Keyboard::D:
		mPlayer->setDirection(Player::Direction::RIGHT, false);
		break;
	default:
		break;
	}
}

/**
 * Converts a screen coordinate to a world coordinate.
 */
sf::Vector2<float>
Game::convertCoordinates(int x, int y) {
	return mWindow.convertCoords(sf::Vector2i(x, y), mView);
}

void
Game::mouseDown(const sf::Event& event) {
	switch(event.mouseButton.button) {
	case sf::Mouse::Left:
		mPlayer->pullTrigger();
		break;
	case sf::Mouse::Right:
		mPlayer->setDestination(convertCoordinates(event.mouseButton.x,
				event.mouseButton.y));
		break;
	default:
		break;
	}
}

/**
 * Handles mouse key up events.
 */
void
Game::mouseUp(const sf::Event& event) {
	switch (event.mouseButton.button) {
	case sf::Mouse::Left:
		mPlayer->releaseTrigger();
		break;
	default:
		break;
	}
}

/**
 * Renders world and GUI.
 */
void
Game::render() {
	mWindow.clear();

	mView.setCenter(mPlayer->getPosition());

	// Render world and dynamic stuff.
	mWindow.setView(mView);

	mWindow.draw(mWorld);

	// Render GUI and static stuff.
	mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}
