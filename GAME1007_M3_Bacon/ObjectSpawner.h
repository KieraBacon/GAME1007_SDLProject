#pragma once
#include <vector>
#include "Enemy.h"
class ObjectSpawner
{
	static SDL_Rect* m_pWorld;	// Pointer to the world
	std::vector<Enemy*>* m_pVec;// Pointer to the vector that holds spawned objects
	Enemy* m_pEnemy;			// Pointer to the template Enemy for spawned objects
	int m_iRespawnCD;			// Number of frames since last spawn
	int m_iRespawnRate;			// Number of frames between spawns
public:
	// Default constructor
	ObjectSpawner() {};

	// Base constructor that assigns all members
	ObjectSpawner(std::vector<Enemy*>* vec, Enemy* mob, int rte) :
		m_pVec(vec),
		m_pEnemy(mob),
		m_iRespawnCD(0),
		m_iRespawnRate(rte) {};

	// Static mutator
	static void setWorld(SDL_Rect* w) { m_pWorld = w; }

	// Unique class methods
	void update();
	void spawn();
};