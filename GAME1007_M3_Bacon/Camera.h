#pragma once
#include <SDL.h>
#include "Player.h"
#include <iostream>
class Camera
{
	Player* m_pPlayer;		// Holds a pointer to the player the camera is following
	SDL_Rect* m_pWorld;		// Holds a pointer to the global game world frame
	SDL_Rect m_rViewport;	// Holds the frame within the gameworld seen though the window
	SDL_Rect m_rOffDst;		// Holds the offset of a game world Rect - the viewport Rect, to be passed to the renderer
public:
	// Default constructor
	Camera() {};

	// Base constructor that assigns all members
	Camera(Player* plr, SDL_Rect* wld, int x, int y, int w, int h) : m_pPlayer(plr), m_pWorld(wld), m_rViewport{ x, y, w, h } {};
	
	// Accessor methods
	int getX() { return m_rViewport.x; };
	int getY() { return m_rViewport.y; };
	int getW() { return m_rViewport.w; };
	int getH() { return m_rViewport.h; };
	SDL_Rect getViewport() { return m_rViewport; };

	// Mutator methods
	void setX(int x) { m_rViewport.x = x; }
	void setY(int y) { m_rViewport.y = y; }
	void setW(int w) { m_rViewport.w = w; }
	void setH(int h) { m_rViewport.h = h; }

	// Unique class methods
	SDL_Rect* getOffset(SDL_Rect* r);
	void update();
};
