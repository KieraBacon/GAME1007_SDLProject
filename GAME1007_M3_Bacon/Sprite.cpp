#include "Sprite.h"

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

void Sprite::renderOffset(SDL_Renderer * renderer, SDL_Rect* camera, int screenW, int screenH)
{
	//SDL_Rect l_rOffset = { m_dst.x - camera->x, m_dst.y - camera->y, m_dst.w / camera->w / screenW, m_dst.h / camera->h / screenH };
	//SDL_Rect l_rOffset = { m_dst.x, m_dst.y, m_dst.w, m_dst.h };
	SDL_Rect l_rOffset = { m_dst.x - camera->x, m_dst.y - camera->y, m_dst.w, m_dst.h };
	SDL_RenderCopy(renderer, m_tex, &m_src, &l_rOffset);
}
