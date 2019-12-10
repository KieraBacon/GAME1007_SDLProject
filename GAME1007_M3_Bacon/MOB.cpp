#include "MOB.h"
#include <cmath>		// Used for sin and cos functions
#include <iostream>
#define PI 3.14159265	// Value of PI needed for sin and cos functions

SDL_Rect* MOB::m_pWorld = nullptr;

MOB::MOB(const Sprite& base, int x, int y, int spd, float ang) :
	Sprite(base, x, y), m_iSpeed(spd), m_fAngle(ang) {}

MOB::MOB(int src_x, int src_y, int src_w, int src_h,
	int dst_x, int dst_y, int dst_w, int dst_h,
	SDL_Texture* tex, int spriteMax, int frameMax,
	int spd, float ang) :
	Sprite(src_x, src_y, src_w, src_h,
		dst_x, dst_y, dst_w, dst_h, tex,
		spriteMax, frameMax),
	m_iSpeed(spd), m_fAngle(ang) {}

void MOB::update()
{
	// Update position
	m_dst.x += m_iSpeed * static_cast<int>(cos(m_fAngle * PI / 180));
	m_dst.y -= m_iSpeed * static_cast<int>(sin(m_fAngle * PI / 180));

	// Check if off-screen
	if (m_dst.x + m_dst.w < m_pWorld->x ||
		m_dst.x > m_pWorld->w ||
		m_dst.y + m_dst.h < m_pWorld->y ||
		m_dst.y > m_pWorld->h)
		m_bActive = false;
}
