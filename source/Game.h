/*
 * Game.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#ifndef DG_GAME_H_
#define DG_GAME_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include <Box2D/Box2D.h>

#include "Pathfinder.h"
#include "TileManager.h"
#include "sprite/Player.h"
#include "util/Collection.h"

class Player;
class Collection;

/*
 * Use vertex for tiles.
 */
class Game : private sf::NonCopyable, public b2ContactListener {
// Public functions.
public:
	Game(const Vector2i& resolution);
	~Game();

	void loop();
    void BeginContact(b2Contact* contact);

// Private functions.
private:
	void input();
	void render();
	void tick();

	void keyDown(const sf::Event& event);
	void keyUp(const sf::Event& event);
	void mouseUp(const sf::Event& event);

	sf::String getFps();
	sf::Vector2<float> convertCoordinates(int x, int y);

// Private variables.
private:
	static const int FPS_GOAL;
	static const float TICKS_GOAL;

	b2World mWorld;

	sf::RenderWindow mWindow;
	sf::Clock mClock;
	sf::View mView;
	//sf::Text mFps;

	Collection mCollection;
	TileManager mTileManager;
	Pathfinder mPathfinder;
	Player mPlayer;

	/// Milliseconds since the last tick.
	sf::Uint32 mElapsed;

	bool mQuit;
	bool mPaused;
};


#endif /* DG_GAME_H_ */
