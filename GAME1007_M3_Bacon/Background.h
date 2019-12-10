#pragma once
#include "Sprite.h"
class Background :
	public Sprite
{
	SDL_Rect m_dst2;
	int m_iMoveCD;
	int m_iMoveRate;
	int m_iMoveDistance;
public:
	// Default construcot
	Background() {};

	// Base constructor that assigns all members
	Background(int mr, int md, int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture* tex,
		int spriteMax, int frameMax) :
		Sprite(src_x, src_y, src_w, src_h,
			dst_x, dst_y, dst_w, dst_h, tex,
			spriteMax, frameMax),
		m_iMoveCD(0),
		m_iMoveRate(mr),
		m_iMoveDistance(md)
	{
		m_dst2 = { m_dst.w, m_dst.y, m_dst.w, m_dst.h };
	};

	// Accessor methods
	SDL_Rect* getDst2() { return &m_dst2; }

	// Unique class methods
	void update(int screenH, int playerY);
};