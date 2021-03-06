/*
 * Game.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#ifndef DG_GAME_H_
#define DG_GAME_H_

#include <TGUI/TGUI.hpp>

#include <LTBL/Light/LightSystem.h>
#include <LTBL/Light/Light_Point.h>

#include "generator/Generator.h"
#include "Pathfinder.h"
#include "World.h"

class Player;

/*
 * High level game managing, including game loop, input, high
 * level rendering, and general class handling
 */
class Game : private sf::NonCopyable {
public:
	explicit Game(tgui::Window& window);
	~Game();

	void loop();

private:
	void input();
	void render();

	void keyDown(const sf::Event& event);
	void keyUp(const sf::Event& event);
	void mouseDown(const sf::Event& event);
	void mouseUp(const sf::Event& event);

	Vector2<float> convertCoordinates(int x, int y);
	void updateGui();
	void initPlayer();
	void initLight();
	void insertEnemies(const std::vector<Vector2f>& positions);

private:
	static const int FPS_GOAL = 60;

	tgui::Window& mWindow;
	sf::Clock mClock;
	sf::View mWorldView;
	tgui::Label* mHealth;
	tgui::Label* mAmmo;
	tgui::Label* mCurrentWeapon;
	tgui::Label* mLeftGadget;
	tgui::Label* mRightGadget;
	tgui::Label* mPickupInstruction;
	std::shared_ptr<sf::Texture> mCrosshairTexture;
	sf::Sprite mCrosshair;

	World mWorld;
	Pathfinder mPathfinder;
	ltbl::LightSystem mLightSystem;
	Generator mGenerator;
	ltbl::Light_Point* mPlayerAreaLight = new ltbl::Light_Point();
	ltbl::Light_Point* mPlayerDirectionLight = new ltbl::Light_Point();

	std::shared_ptr<Player> mPlayer;

	bool mQuit = false;
	bool mPaused = false;
};

#endif /* DG_GAME_H_ */
