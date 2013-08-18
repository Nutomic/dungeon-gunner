/*
 * Game.cpp
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#include "Game.h"

#include <Thor/Vectors.hpp>

#include "generator/Generator.h"
#include "items/Heal.h"
#include "items/Shield.h"
#include "sprites/Enemy.h"
#include "sprites/Player.h"
#include "util/Yaml.h"

const int Game::FPS_GOAL = 60;

void Game::initPlayer() {
	mPlayer = std::shared_ptr < Player
			> (new Player(mWorld, mPathfinder, mGenerator.getPlayerSpawn()));
	mPlayer->setLeftGadget(std::shared_ptr < Gadget > (new Heal()));
	mPlayer->setRightGadget(std::shared_ptr < Gadget > (new Shield()));
	mWorld.insertCharacter(mPlayer);
}

/**
 * Initializes game, including window and objects (sprites).
 */
Game::Game(tgui::Window& window) :
		mWindow(window),
		mWorldView(Vector2f(0, 0), mWindow.getView().getSize()),
		mGenerator(mWorld, mPathfinder),
		mQuit(false),
		mPaused(false) {
	mWindow.setFramerateLimit(FPS_GOAL);
	mWindow.setKeyRepeatEnabled(false);

	mGenerator.generateCurrentAreaIfNeeded(Vector2f());
	initPlayer();

	mHealth = window.add<tgui::Label>();
	mHealth->setTextSize(20);
	mAmmo = window.add<tgui::Label>();
	mAmmo->setTextSize(20);
	mCurrentWeapon = window.add<tgui::Label>();
	mCurrentWeapon->setTextSize(14);
	mLeftGadget = window.add<tgui::Label>();
	mLeftGadget->setTextSize(14);
	mRightGadget = window.add<tgui::Label>();
	mRightGadget->setTextSize(14);
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
		if (mPlayer->getHealth() == 0) {
			Vector2f pos = mPlayer->getCrosshairPosition();
			mWorld.remove(mPlayer);
			initPlayer();
			mPlayer->setCrosshairPosition(pos);
		}

		mWorld.step(elapsed);

		updateGui();

		render();

		mGenerator.generateCurrentAreaIfNeeded(mPlayer->getPosition());
	}
}

/**
 * Displays current player ammo in the ammo widget.
 */
void
Game::updateGui() {
	int mag = mPlayer->getMagazineAmmo();
	int total = mPlayer->getTotalAmmo();

	std::string magString = tgui::to_string(mag);
	if (mag < 10) magString = "0" + magString;

	std::string totalString = tgui::to_string(total);
	if (total < 100) totalString = "0" + totalString;
	if (total < 10) totalString = "0" + totalString;

	mHealth->setText(tgui::to_string(mPlayer->getHealth()));
	mAmmo->setText(magString + "/" + totalString);
	mCurrentWeapon->setText(mPlayer->getWeaponName());
	mLeftGadget->setText(mPlayer->getLeftGadgetName());
	mRightGadget->setText(mPlayer->getRightGadgetName());

	mHealth->setPosition(0, mWindow.getSize().y - mHealth->getSize().y);
	mAmmo->setPosition(mWindow.getSize().x - mAmmo->getSize().x,
			mWindow.getSize().y - mAmmo->getSize().y);
	mCurrentWeapon->setPosition(mWindow.getSize().x - mCurrentWeapon->getSize().x,
			mAmmo->getPosition().y - mCurrentWeapon->getSize().y);
	mLeftGadget->setPosition(mWindow.getSize().x / 2 - mLeftGadget->getSize().x - 10,
			mWindow.getSize().y - mLeftGadget->getSize().y);
	mRightGadget->setPosition(mWindow.getSize().x / 2 + 10,
			mWindow.getSize().y - mRightGadget->getSize().y);
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
		case sf::Event::MouseWheelMoved:
			mPlayer->toggleWeapon();
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
	case sf::Keyboard::F:
		mPlayer->pickUpItem();
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
	case sf::Keyboard::Q:
		mPlayer->useLeftGadget();
		break;
	case sf::Keyboard::E:
		mPlayer->useRightGadget();
		break;
	case sf::Keyboard::R:
		mPlayer->reload();
		break;
	case sf::Keyboard::Num1:
		mPlayer->selectFirstWeapon();
		break;
	case sf::Keyboard::Num2:
		mPlayer->selectSecondWeapon();
		break;
	default:
		break;
	}
}

/**
 * Converts a screen coordinate to a world coordinate.
 */
Vector2<float>
Game::convertCoordinates(int x, int y) {
	return mWindow.mapPixelToCoords(Vector2i(x, y), mWorldView);
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

	mWorldView.setCenter(mPlayer->getPosition());

	// Render world and dynamic stuff.
	mWindow.setView(mWorldView);
	mWindow.draw(mWorld);

	// Render GUI and static stuff.
	mWindow.setView(mWindow.getDefaultView());
	mWindow.drawGUI();

	mWindow.display();
}
