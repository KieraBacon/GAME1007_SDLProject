#pragma once
#include <vector>
#include "Enemy.h"
template <class T>
class ObjectSpawner
{
	static SDL_Rect* m_pWorld;	// Pointer to the world
	std::vector<T*>* m_pVec;	// Pointer to the vector that holds spawned objects
	T m_pObj;					// Pointer to the template Enemy for spawned objects
	int m_iRespawnCD;			// Number of frames since last spawn
	int m_iRespawnRate;			// Number of frames between spawns
	int m_iSpawnMin;			// Minimum number of objects per spawn
	int m_iSpawnMax;			// Maximum number of objects per spawn
	int m_iGridSize;			// Number of pixels per vertex of the grid
public:
	// Default constructor
	ObjectSpawner() {};

	// Base constructor that assigns all members
	ObjectSpawner(std::vector<T*>* vec, const T& obj, int min, int max, int rte, int grd) :
		m_pVec(vec),
		m_pObj(obj),
		m_iSpawnMin(min),
		m_iSpawnMax(max),
		m_iRespawnCD(0),
		m_iRespawnRate(rte),
		m_iGridSize(grd) {};

	// Static mutator
	static void setWorld(SDL_Rect* w) { m_pWorld = w; }

	// Unique class methods
	void update();
	void spawn();
};

template <class T>
SDL_Rect* ObjectSpawner<T>::m_pWorld = nullptr;

template <class T>
void ObjectSpawner<T>::update()
{
	if (m_iRespawnCD < m_iRespawnRate)
		m_iRespawnCD++;
	else
	{
		m_iRespawnCD = 0;
		spawn();
	}
}

void ObjectSpawner<Enemy>::spawn()
{
	m_iRespawnCD = 0;

	int qty = m_iSpawnMin + rand() % (m_iSpawnMax - m_iSpawnMin);
	for (int i = 0; i < qty; i++)
	{
		int gridVertex = rand() % (m_pWorld->h / m_iGridSize) * m_iGridSize;
		m_pVec->push_back(new Enemy(m_pObj, m_pWorld->w, gridVertex, m_pObj.getMR(), m_pObj.getSpeed(), m_pObj.getAngle()));
	}
}

void ObjectSpawner<Projectile>::spawn()
{
	m_iRespawnCD = 0;

	int qty = m_iSpawnMin + rand() % (m_iSpawnMax - m_iSpawnMin);
	for (int i = 0; i < qty; i++)
	{
		int gridVertex = rand() % (m_pWorld->h / m_iGridSize) * m_iGridSize;
		m_pVec->push_back(new Projectile(m_pObj, m_pWorld->w, gridVertex, m_pObj.getMR(), m_pObj.getSpeed(), m_pObj.getAngle(), m_pObj.getDamage()));
	}
}