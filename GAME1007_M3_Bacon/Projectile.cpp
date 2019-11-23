#include "Projectile.h"
#include <cmath>		// Used for sin and cos functions
#define PI 3.14159265

void Projectile::update(int xMax, int yMax)
{
	// Update position
	if (m_fAngle == 0)				// If bullet is moving directly right
		m_dst.x += m_iSpeed;
	else if (m_fAngle == 180)		// If bullet is moving directly left
		m_dst.x -= m_iSpeed;
	else							// If bullet is moving at an angle
	{
		m_dst.x += static_cast<int>(cos(m_fAngle * PI / 180));
		m_dst.y -= static_cast<int>(sin(m_fAngle * PI / 180));
	}

	// Check if off-screen
	if (m_dst.x < 0 || m_dst.x > xMax || m_dst.y < 0 || m_dst.y > yMax)
		m_bActive = false;

	animate();
}