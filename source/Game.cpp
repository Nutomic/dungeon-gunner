/*
 * Game.cpp
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#include "Game.h"

#include "abstract/Actor.h"
#include "sprite/Cover.h"
#include "sprite/Enemy.h"
#include "util/Loader.h"
#include "util/ResourceManager.h"
#include "util/String.h"

/// Goal amount of frames per second.
const int Game::FPS_GOAL = 60;

/// Milliseconds per tick at FPS_GOAL.
const float Game::TICKS_GOAL = 1000 / Game::FPS_GOAL;

/**
 * Initializes game, including window and objects (sprites).
 */
Game::Game(const Vector2i& resolution) :
		mWorld(b2Vec2(0, 0)),
		mWindow(sf::VideoMode(resolution.x, resolution.y, 32), "Roguelike Shooter",
				sf::Style::Close | sf::Style::Titlebar),
		mView(Vector2f(0, 0), Vector2f(resolution)),
		//mFps("test"),
		mTileManager(mWorld),
		mPlayer(mWorld, mCollection, Vector2f(200.0f, 100.0f)),
		mElapsed(0),
		mQuit(false),
		mPaused(false) {
	mWindow.setFramerateLimit(FPS_GOAL);
	mWindow.setKeyRepeatEnabled(true);
	mWorld.SetContactListener(this);

	mTileManager.generate();
	for (int i = 0; i < 500; i += 50) {
		mCollection.insert(std::shared_ptr<Sprite>(new Cover(Vector2f(i, i), Vector2i(20, 20),
				mWorld)), Collection::LEVEL_STATIC);
	}
	mCollection.insert(std::shared_ptr<Sprite>(new Enemy(mWorld, Vector2f(400.0f, 200.0f))),
			Collection::LEVEL_ACTOR);
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
	sf::Uint32 left = 0;
	while (!mQuit) {

		input();

		for (; !mPaused && (left >= TICKS_GOAL); left -= TICKS_GOAL) {
			Actor::think(TICKS_GOAL);
			mWorld.Step(1.0f / FPS_GOAL, 8, 3);

			mCollection.checkDelete();
		}

		//mFps.setString(getFps());

		tick();
		left += mElapsed;

		render();
	}
}

/**
 * Saves ticks since last call.
 */
void
Game::tick() {
	mElapsed = mClock.restart().asMilliseconds();
	if (mPaused) {
		mElapsed = 0;
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
		case sf::Event::MouseButtonReleased:
			mouseUp(event);
			break;
		case sf::Event::MouseMoved:
			mPlayer.setCrosshairPosition(convertCoordinates(event.mouseMove.x, event.mouseMove.y));
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
		mPlayer.setDirection(Player::DIRECTION_UP, true);
		break;
	case sf::Keyboard::S:
		mPlayer.setDirection(Player::DIRECTION_DOWN, true);
		break;
	case sf::Keyboard::A:
		mPlayer.setDirection(Player::DIRECTION_LEFT, true);
		break;
	case sf::Keyboard::D:
		mPlayer.setDirection(Player::DIRECTION_RIGHT, true);
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
		mPlayer.setDirection(Player::DIRECTION_UP, false);
		break;
	case sf::Keyboard::S:
		mPlayer.setDirection(Player::DIRECTION_DOWN, false);
		break;
	case sf::Keyboard::A:
		mPlayer.setDirection(Player::DIRECTION_LEFT, false);
		break;
	case sf::Keyboard::D:
		mPlayer.setDirection(Player::DIRECTION_RIGHT, false);
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
	return mWindow.convertCoords(Vector2i(x, y), mView);
}

/**
 * Handles mouse key up events.
 */
void
Game::mouseUp(const sf::Event& event) {
	switch (event.mouseButton.button) {
	case sf::Mouse::Left:
		mPlayer.fire();
		break;
	case sf::Mouse::Right:
		mPlayer.move(convertCoordinates(event.mouseButton.x, event.mouseButton.y));
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

	mView.setCenter(mPlayer.getPosition());

	// Render world and dynamic stuff.
	mWindow.setView(mView);

	mWindow.draw(mTileManager);
	mWindow.draw(mCollection);
	mWindow.draw(mPlayer);

	// Render GUI and static stuff.
	mWindow.setView(mWindow.getDefaultView());

	//mWindow.draw(mFps);

	mWindow.display();
}

/**
 * Returns current FPS as string.
 */
sf::String
Game::getFps() {
	return str((mElapsed != 0) ? 1000.0f / mElapsed : 0.0f, 2);
}

/**
 * Begin of collision, call callback function on both objects.
 */
void
Game::BeginContact(b2Contact* contact) {
	Physical& first = *static_cast<Physical*>(contact->GetFixtureA()->GetBody()->GetUserData());
	Physical& second = *static_cast<Physical*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (!first.doesCollide(second) || !second.doesCollide(first)) {
		contact->SetEnabled(false);
		return;
	}
	first.onCollide(second, second.getCategory());
	second.onCollide(first, first.getCategory());
}
