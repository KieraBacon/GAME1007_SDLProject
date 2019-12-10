#include "Sprite.h"
#include <cmath>		// Used for sin and cos functions
#define PI 3.14159265	// Value of PI needed for sin and cos functions

Sprite::Sprite(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex, int sm, int fm, int rm, bool e) :
	m_src(src), m_dst(dst), m_pTex(tex), m_iSpriteMax(sm), m_iFrameMax(fm), m_iRowMax(rm), m_bExpires(e) {}

Sprite::Sprite(const Sprite& base, int x, int y) :
	m_src(base.m_src), m_dst{ x, y, base.m_dst.w, base.m_dst.h }, m_pTex(base.m_pTex),
	m_iSpriteMax(base.m_iSpriteMax), m_iFrameMax(base.m_iFrameMax), m_iRowMax(base.m_iRowMax), m_bExpires(base.m_bExpires) {}

void Sprite::animate()
{
	if (m_iFrameMax != -1)
	{
		if (m_iFrame == m_iFrameMax)
		{
			m_iFrame = 0;		// Roll the frame counter back to 0
			m_iSprite++;		// Increment the sprite index
			if (m_iSprite == m_iSpriteMax)
			{
				m_iSprite = 0;		// Roll the sprite back to 0
				m_iRow++;			// Increment the row index
				if (m_iRow == m_iRowMax)
					m_bExpires ? m_bActive = false : m_iRow = 0;	// Expire or roll the row back to 0
				m_src.y = m_src.h * m_iRow;	// Set the new source.y
			}
			m_src.x = m_src.w * m_iSprite;	// Set the new source.x
		}
		else
			m_iFrame++;			// Increment the frame counter
	}
}





SDL_Rect* Background::m_pWorld = nullptr;
SDL_Rect* Background::m_pCamera = nullptr;

Background::Background(SDL_Rect src, SDL_Rect dst, SDL_Texture * tex, int sm, int fm, int mr, int spd) :
	Sprite(src, dst, tex, sm, fm), m_dst2{ dst.w, dst.y, dst.w, dst.h }, m_iMoveCD(0), m_iMoveRate(mr), m_iSpeed(spd) {}

void Background::update()
{
	// Update the x position only on frame count
	if (m_iMoveCD == m_iMoveRate)
	{
		m_iMoveCD = 0;
		m_dst.x -= m_iSpeed;
		m_dst2.x -= m_iSpeed;
		if (m_dst.x <= 0 - m_dst.w)			// If the first background is fully off screen to the left
			m_dst.x = m_dst2.x + m_dst2.w;	// Move it to the right of the second background
		if (m_dst2.x <= 0 - m_dst2.w)		//  ' '	  second background
			m_dst2.x = m_dst.x + m_dst.w;	//  ' '						   first background
	}
	else
		m_iMoveCD++;
}





SDL_Rect* MOB::m_pWorld = nullptr;

MOB::MOB(const Sprite& base, int x, int y, SDL_Rect col, Mix_Chunk* esnd, int mr, int spd, float ang) :
	Sprite(base, x, y), m_col(col), m_pSnd_End(esnd), m_iMoveRate(mr), m_iMoveCD(0), m_iSpeed(spd), m_fAngle(ang) {}

MOB::MOB(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm, Mix_Chunk* esnd, int mr, int spd, float ang) :
	Sprite(src, dst, tex, sm, fm), m_col(col), m_pSnd_End(esnd), m_iMoveRate(mr), m_iMoveCD(0), m_iSpeed(spd), m_fAngle(ang) {}

SDL_Rect* MOB::getCol()
{
	m_col_dst = { m_dst.x + m_col.x, m_dst.y + m_col.y, m_col.w, m_col.h };
	return &m_col_dst;
}

void MOB::update()
{
	// Update position only on frame count
	if (m_iMoveCD == m_iMoveRate)
	{
		m_iMoveCD = 0;
		m_dst.x += m_iSpeed * static_cast<int>(cos(m_fAngle * PI / 180));
		m_dst.y -= m_iSpeed * static_cast<int>(sin(m_fAngle * PI / 180));
	}
	else
		m_iMoveCD++;

	// Check if off-screen
	if (m_dst.x + m_dst.w < m_pWorld->x ||
		m_dst.x > m_pWorld->w ||
		m_dst.y + m_dst.h < m_pWorld->y ||
		m_dst.y > m_pWorld->h)
		m_bActive = false;
}





Projectile::Projectile(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm,
	Mix_Chunk* ssnd, Mix_Chunk* esnd, int mr, int spd, float ang, int dmg) :
	MOB(src, dst, col, tex, sm, fm, esnd, mr, spd, ang), m_pSnd_Start(ssnd), m_iDamage(dmg) {}

Projectile::Projectile(const Projectile & base, int x, int y, int mr, int spd, float ang, int dmg) :
	MOB(base.m_src, SDL_Rect{ x, y, base.m_dst.w, base.m_dst.h }, base.m_col,
		base.m_pTex, base.m_iSpriteMax, base.m_iFrameMax, base.m_pSnd_End, mr, spd, ang),
	m_pSnd_Start(base.m_pSnd_Start), m_iDamage(dmg) {}
