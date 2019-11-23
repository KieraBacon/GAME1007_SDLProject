#include "Background.h"

void Background::update(int screenH, int playerY)
{
	// Update the x position only on frame count
	if (m_iMoveCD == m_iMoveRate)
	{
		m_iMoveCD = 0;
		m_dst.x -= m_iMoveDistance;
		m_dst2.x -= m_iMoveDistance;
		if (m_dst.x <= 0 - m_dst.w)			// If the first background is fully off screen to the left
			m_dst.x = m_dst2.x + m_dst2.w;	// Move it to the right of the second background
		if (m_dst2.x <= 0 - m_dst2.w)		//  ' '	  second background
			m_dst2.x = m_dst.x + m_dst.w;	//  ' '						   first background
	}
	else
		m_iMoveCD++;
}