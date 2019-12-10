#include "Camera.h"

SDL_Rect* Camera::getOffset(SDL_Rect* r)
{
	m_rOffDst = { r->x - m_rViewport.x, r->y - m_rViewport.y, r->w, r->h };
	return &m_rOffDst;
}

void Camera::update()
{
	m_rViewport.y = static_cast<int>(
		(static_cast<double>(m_pPlayer->getY()) + static_cast<double>(m_pPlayer->getH()) / 2.0)  *
		((static_cast<double>(m_pWorld->h) - static_cast<double>(m_rViewport.h)) / static_cast<double>(m_pWorld->h))
		);
}
