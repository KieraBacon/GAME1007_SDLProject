#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Sprite.h"

class Background :
	public Sprite
{
private:
	SDL_Rect m_dst2;
	int m_iMoveCD;
	int m_iMoveRate;
	int m_iMoveDistance;
public:
	Background() {};
	Background(int mr, int md, int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture* tex,
		int spriteMax = 0, int frameMax = 0, int sprite = 0, int frame = 0) :
		Sprite(src_x, src_y, src_w, src_h,
			dst_x, dst_y, dst_w, dst_h, tex,
			spriteMax, frameMax, sprite, frame),
		m_iMoveCD(0),
		m_iMoveRate(mr),
		m_iMoveDistance(md)
	{
		m_dst2 = { m_dst.w, m_dst.y, m_dst.w, m_dst.h };
	};
	void update(int screenH, int playerY);
	SDL_Rect* getDst2() { return &m_dst2; }
};

