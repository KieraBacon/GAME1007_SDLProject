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