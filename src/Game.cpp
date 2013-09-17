/*
 * Game.cpp
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#include "Game.h"

#include <Thor/Vectors.hpp>

#include <LTBL/Utils.h>

#include "generator/Generator.h"
#include "sprites/Enemy.h"
#include "sprites/Player.h"
#include "sprites/items/HealthOrb.h"
#include "util/Angles.h"
#include "util/Loader.h"
#include "util/Yaml.h"

/**
 * Initializes game, including window and objects (sprites).
 */
Game::Game(tgui::Window& window) :
		mWindow(window),
		mWorldView(Vector2f(0, 0), mWindow.getView().getSize()),
		mLightSystem(AABB(Vec2f(-100000, -100000), Vec2f(100000, 10000)), &window,
				"res/textures/light_fin.png", "res/shaders/light_attenuation_shader.frag"),
		mGenerator(mWorld, mPathfinder, mLightSystem, Yaml("generation.yaml")) {
	mWindow.setFramerateLimit(FPS_GOAL);
	mWindow.setKeyRepeatEnabled(false);
	srand(time(nullptr));

	initPlayer();
	initLight();

	mCrosshairTexture = Loader::i().fromFile<sf::Texture>("crosshair.png");
	mCrosshair.setTexture(*mCrosshairTexture, true);
	mWindow.setMouseCursorVisible(false);

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
	mPickupInstruction = window.add<tgui::Label>();
	mPickupInstruction->setTextSize(14);
}

/**
 * Closes window.
 */
Game::~Game() {
	mWindow.close();
}

void
Game::initPlayer() {
	Character::EquippedItems playerItems = {
			Weapon::WeaponType::PISTOL,	Weapon::WeaponType::KNIFE,
			Gadget::GadgetType::NONE, Gadget::GadgetType::NONE
	};
	auto enemySpawns = mGenerator.generateCurrentAreaIfNeeded(Vector2f());
	mPlayer = std::shared_ptr<Player>(new Player(mWorld, mPathfinder,
			mGenerator.getPlayerSpawn(), playerItems));
	mWorld.insertCharacter(mPlayer);
	insertEnemies(enemySpawns);
}

/**
 * Inserts enemies at the given positions, if they are more than
 * Character::VISION_DISTANCE away from the player's postion.
 */
void
Game::insertEnemies(const std::vector<Vector2f>& positions) {
	for (const auto& spawn : positions) {
		if (thor::length(spawn - mPlayer->getPosition()) >
				Character::VISION_DISTANCE)
			mWorld.insertCharacter(std::make_shared<Enemy>(mWorld,
					mPathfinder, spawn, mPlayer->getEquippedItems()));
	}
}

/**
 * Initializes the lights held by the player and sets light system parameters.
 */
void
Game::initLight() {
	Yaml config("light.yaml");
	Color3f lightColor(config.get("color_red", 0) / 255.0f,
			config.get("color_green", 0) / 255.0f,
			config.get("color_blue", 0) / 255.0f);
	mLightSystem.m_checkForHullIntersect = false;
	mLightSystem.m_useBloom = false;

	mPlayerAreaLight->m_radius = 250.0f;
    mPlayerAreaLight->m_size = 1.0f;
    mPlayerAreaLight->m_softSpreadAngle = 0;
    mPlayerAreaLight->m_spreadAngle = 2.0f * ltbl::pi;
    mPlayerAreaLight->m_intensity = 1.1f;
    mPlayerAreaLight->m_bleed = 0;
    mPlayerAreaLight->m_color = lightColor;
	mPlayerDirectionLight->m_linearizeFactor = 0.5;
	mPlayerAreaLight->CalculateAABB();
	mLightSystem.AddLight(mPlayerAreaLight);

	mPlayerDirectionLight->m_radius = 500.0f;
	mPlayerDirectionLight->m_size = 25.0f;
	mPlayerDirectionLight->m_softSpreadAngle = 0.1f * ltbl::pi;
	mPlayerDirectionLight->m_spreadAngle =
			degreeToRadian(config.get("light_cone_angle", 0.0f));
	mPlayerDirectionLight->m_intensity = 5;
	mPlayerDirectionLight->m_bleed = 0;
	mPlayerDirectionLight->m_color = lightColor;
	mPlayerDirectionLight->m_linearizeFactor = 1;
	mPlayerDirectionLight->CalculateAABB();
	mLightSystem.AddLight(mPlayerDirectionLight);
}

/**
 * Runs the game loop.
 */
void
Game::loop() {
	while (!mQuit) {
		input();

		int elapsed = (mPaused)
				? 0
				: mClock.restart().asMilliseconds();

		mWorld.think(elapsed);
		// Respawn player at start position on death.
		if (mPlayer->getHealth() <= 0) {
			Vector2f pos = mPlayer->getCrosshairPosition();
			mWorld.remove(mPlayer);
			initPlayer();
			mPlayer->setCrosshairPosition(pos);
		}

		mWorld.step(elapsed);

		updateGui();

		render();

		auto enemySpawns = mGenerator.generateCurrentAreaIfNeeded(mPlayer->getPosition());
		insertEnemies(enemySpawns);
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

	auto item = mWorld.getClosestItem(mPlayer->getPosition());
	auto orb = std::dynamic_pointer_cast<HealthOrb>(item);
	if (orb && mPlayer->getHealth() < mPlayer->getMaxHealth()) {
		mPlayer->onDamage(- orb->getAmountHealed());
		mWorld.remove(item);
	}
	else if (item) {
		mPickupInstruction->setText("F - pick up " + item->getName());
		mPickupInstruction->setPosition(
				mWindow.getSize().x / 2 - mPickupInstruction->getSize().x / 2,
				mWindow.getSize().y * 0.66f);
		mPickupInstruction->show();
	}
	else
		mPickupInstruction->hide();
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
			mCrosshair.setPosition(Vector2f(sf::Mouse::getPosition(mWindow) -
					Vector2i(mCrosshair.getTextureRect().width, mCrosshair.getTextureRect().height) / 2));
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

	// Update light
	mPlayerAreaLight->SetCenter(mPlayer->getPosition().toVec2f());
	mPlayerDirectionLight->SetCenter(mPlayer->getPosition().toVec2f());
	mPlayerDirectionLight->SetDirectionAngle(degreeToRadian(90 - mPlayer->getDirection()));

	mLightSystem.SetView(mWorldView);
	mLightSystem.RenderLights();
	mLightSystem.RenderLightTexture();

	// Render GUI and static stuff.
	mWindow.setView(mWindow.getDefaultView());
	mWindow.drawGUI();
	mWindow.draw(mCrosshair);

	mWindow.display();
}
