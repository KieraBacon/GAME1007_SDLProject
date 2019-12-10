#include "Sprite.h"

Sprite::Sprite(int src_x, int src_y, int src_w, int src_h,
	int dst_x, int dst_y, int dst_w, int dst_h,
	SDL_Texture* tex, int spriteMax, int frameMax) :
	m_src{ src_x, src_y, src_w, src_h },
	m_dst{ dst_x, dst_y, dst_w, dst_h },
	m_tex(tex),
	m_spriteMax(spriteMax),
	m_frameMax(frameMax) {}

Sprite::Sprite(int x, int y, int w, int h,
	SDL_Texture* tex, int spriteMax, int frameMax) :
	m_src{ 0, 0, w, h },
	m_dst{ x, y, w, h },
	m_tex(tex),
	m_spriteMax(spriteMax),
	m_frameMax(frameMax) {}

Sprite::Sprite(const Sprite& base, int x, int y) :
	m_src{ base.m_src.x, base.m_src.y, base.m_src.w, base.m_src.h },
	m_dst{ x, y, base.m_dst.w, base.m_dst.h },
	m_tex(base.m_tex),
	m_spriteMax(base.m_spriteMax),
	m_frameMax(base.m_frameMax) {}

void Sprite::animate()
{
	if (m_spriteMax != 0)
	{
		if (m_frame == m_frameMax)
		{
			m_frame = 0;		// Roll the frame counter back to 0
			m_sprite++;			// Increment the sprite index
			if (m_sprite == m_spriteMax)
				m_sprite = 0;	// Roll the sprite back to 0
			m_src.x = m_src.w * m_sprite;	// Set the new source.x
		}
		else
			m_frame++;			// Increment the frame counter
	}
}