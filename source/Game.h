/*
 * Game.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#ifndef DG_GAME_H_
#define DG_GAME_H_

#include <string>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include "sprites/TileManager.h"
#include "sprites/Player.h"
#include "World.h"

class Player;
class World;

/*
 * Use vertex for tiles.
 */
class Game : private sf::NonCopyable {
// Public functions.
public:
	Game(sf::RenderWindow& window);
	~Game();

	void loop();

// Private functions.
private:
	void input();
	void render();

	void keyDown(const sf::Event& event);
	void keyUp(const sf::Event& event);
	void mouseDown(const sf::Event& event);
	void mouseUp(const sf::Event& event);

	void generate();
	sf::Vector2<float> convertCoordinates(int x, int y);

// Private variables.
private:
	static const int FPS_GOAL;

	sf::RenderWindow& mWindow;
	sf::Clock mClock;
	sf::View mView;

	World mWorld;
	TileManager mTileManager;
	std::shared_ptr<Player> mPlayer;

	bool mQuit;
	bool mPaused;
};


#endif /* DG_GAME_H_ */
