/*
 * Game.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#ifndef DG_GAME_H_
#define DG_GAME_H_

#include "sprites/TileManager.h"
#include "Pathfinder.h"
#include "World.h"

class TileManager;
class Pathfinder;
class Player;
class World;

/*
 * High level game managing, including game loop, input, high
 * level rendering, and general class handling
 */
class Game : private sf::NonCopyable {
public:
	explicit Game(sf::RenderWindow& window);
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

private:
	static const int FPS_GOAL;

	sf::RenderWindow& mWindow;
	sf::Clock mClock;
	sf::View mView;

	World mWorld;
	TileManager mTileManager;
	Pathfinder mPathfinder;
	std::shared_ptr<Player> mPlayer;

	bool mQuit;
	bool mPaused;
};

#endif /* DG_GAME_H_ */
