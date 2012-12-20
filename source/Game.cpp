/*
 * Game.cpp
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#include "Game.h"

#include <Thor/Graphics.hpp>

#include "abstract/Character.h"
#include "sprites/Enemy.h"
#include "types/Instances.h"
#include "types/String.h"
#include "util/Loader.h"
#include "util/ResourceManager.h"

/// Goal amount of frames per second.
const int Game::FPS_GOAL = 60;

/// Milliseconds per tick at FPS_GOAL.
const float Game::TICKS_GOAL = 1000 / Game::FPS_GOAL;

/**
 * Initializes game, including window and objects (sprites).
 */
Game::Game(sf::RenderWindow& window) :
		mWorld(b2Vec2(0, 0)),
		mWindow(window),
		mView(Vector2f(0, 0), mWindow.getView().getSize()),
		//mFps("test"),
		mTileManager(mWorld),
		mPathfinder(mTileManager),
		mElapsed(0),
		mQuit(false),
		mPaused(false) {
	mWindow.setFramerateLimit(FPS_GOAL);
	mWindow.setKeyRepeatEnabled(true);
	mWorld.SetContactListener(this);

	generate();
}

/**
 * Generates a predefined map.
 */
void
Game::generate() {
	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
			mTileManager.setTile(TileManager::TilePosition(x, y), TileManager::Type::WALL);

	for (int x = 1; x < 9; x++)
		for (int y = 1; y < 9; y++)
			mTileManager.setTile(TileManager::TilePosition(x, y), TileManager::Type::FLOOR);

	for (int x = 1; x < 5; x++)
		mTileManager.setTile(TileManager::TilePosition(x, 4), TileManager::Type::WALL);

	Instances instances(mPathfinder, mTileManager, mCollection, mWorld);

	mCollection.insert(std::shared_ptr<Sprite>(new Enemy(instances, Vector2f(400.0f, 200.0f), Yaml("enemy.yaml"))));

	mPlayer = std::unique_ptr<Player>(new Player(instances, Vector2f(200.0f, 100.0f), Yaml("player.yaml")));
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
			Character::think(TICKS_GOAL);
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
			mPlayer->setCrosshairPosition(convertCoordinates(event.mouseMove.x, event.mouseMove.y));
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
	return mWindow.convertCoords(Vector2i(x, y), mView);
}

/**
 * Handles mouse key up events.
 */
void
Game::mouseUp(const sf::Event& event) {
	switch (event.mouseButton.button) {
	case sf::Mouse::Left:
		mPlayer->fire();
		break;
	case sf::Mouse::Right:
		mPlayer->move(convertCoordinates(event.mouseButton.x, event.mouseButton.y));
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

	mWindow.draw(mTileManager);
	mWindow.draw(mCollection);
	mWindow.draw(*mPlayer);

	// Render GUI and static stuff.
	mWindow.setView(mWindow.getDefaultView());

	//mWindow.draw(mFps);

	mWindow.display();
}

/**
 * Returns current FPS as string.
 */
String
Game::getFps() {
	return str((mElapsed != 0) ? 1000.0f / mElapsed : 0.0f, 2);
}

/**
 * Begin of collision, call callback function on both objects.
 */
void
Game::BeginContact(b2Contact* contact) {
	Body& first = *static_cast<Body*>(contact->GetFixtureA()->GetBody()->GetUserData());
	Body& second = *static_cast<Body*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (!first.doesCollide(second) || !second.doesCollide(first)) {
		contact->SetEnabled(false);
		return;
	}
	first.onCollide(second, second.getCategory());
	second.onCollide(first, first.getCategory());
}
