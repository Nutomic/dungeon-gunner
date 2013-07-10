/*
 * Game.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#ifndef DG_GAME_H_
#define DG_GAME_H_

#include <TGUI/TGUI.hpp>

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

	sf::Vector2<float> convertCoordinates(int x, int y);
	void updateGui();

private:
	static const int FPS_GOAL;

	tgui::Window& mWindow;
	sf::Clock mClock;
	sf::View mWorldView;
	tgui::Label* mHealth;
	tgui::Label* mAmmo;
	tgui::Label* mCurrentWeapon;
	tgui::Label* mLeftGadget;
	tgui::Label* mRightGadget;

	World mWorld;
	Pathfinder mPathfinder;
	Generator mGenerator;
	std::shared_ptr<Player> mPlayer;

	bool mQuit;
	bool mPaused;
};

#endif /* DG_GAME_H_ */
