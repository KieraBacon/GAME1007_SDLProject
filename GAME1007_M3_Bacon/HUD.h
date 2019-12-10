#pragma once
#include "Player.h"
class HUD
{
	Player* m_pPlayer;
	SDL_Rect m_rMaxEnergy;
	SDL_Rect m_rCurEnergy;
public:
	// Default constructor
	HUD() {};

	// Base constructor that assigns all members
	HUD(Player* p, int w, int h);

	// Accessor methods
	SDL_Rect* getMax() { return &m_rMaxEnergy; }
	SDL_Rect* getCur() { return &m_rCurEnergy; }

	// Unique class methods
	int update();
};