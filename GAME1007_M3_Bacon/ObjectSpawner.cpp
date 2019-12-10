#include "ObjectSpawner.h"

SDL_Rect* ObjectSpawner::m_pWorld = nullptr;

void ObjectSpawner::update()
{
	if (m_iRespawnCD < m_iRespawnRate)
		m_iRespawnCD++;
	else
	{
		m_iRespawnCD = 0;
		spawn();
	}
}

void ObjectSpawner::spawn()
{
	m_iRespawnCD = 0;
	m_pVec->push_back(new Enemy(*m_pEnemy, m_pWorld->w, m_pWorld->h * (rand() % 100) / 100, 1, 180));
}