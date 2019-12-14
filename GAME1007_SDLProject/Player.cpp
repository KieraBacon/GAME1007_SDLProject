#include "Player.h"

Player::Player(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm, Mix_Chunk* esnd, const Weapon& wpn, int spd, int ngy, int nmx, int rgr, int rgq) :
	Sprite(src, dst, tex, sm, fm),
	m_col(col), m_pSnd_End(esnd), m_pWeapon(wpn), m_iSpeed(spd), m_iEnergy(ngy), m_iEnergyMax(nmx), m_iRegenCD(0), m_iRegenRate(rgr), m_iRegenQty(rgq) {}

SDL_Rect* Player::getCol()
{
	m_col_dst = { m_dst.x + m_col.x, m_dst.y + m_col.y, m_col.w, m_col.h };
	return &m_col_dst;
}

void Player::setEnergy(int e)
{
	if (m_iEnergy + e >= m_iEnergyMax)
		m_iEnergy = m_iEnergyMax;
	else if (m_iEnergy + e <= 0)
	{
		m_iEnergy = 0;
		m_bActive = false;
		Mix_PlayChannel(-1, m_pSnd_End, 0);
		m_src = m_col = { 0, 0, 0, 0 };
		m_iSpeed = m_iRegenQty = 0;
	}
	else
		m_iEnergy += e;
}

void Player::update()
{
	if (m_iRegenCD >= m_iRegenRate && m_iEnergy < m_iEnergyMax)
	{
		m_iRegenCD = 0;
		m_iEnergy + m_iRegenQty >= m_iEnergyMax ? m_iEnergy = m_iEnergyMax : m_iEnergy += m_iRegenQty;
	}
	else if (m_iRegenCD >= m_iRegenRate)
		m_iRegenCD = 0;
	else if (m_iRegenRate != -1)
		m_iRegenCD++;

	// Update weapon
	m_pWeapon.update();

	// Animate
	animate();
}

void Player::fire()
{
	if (m_pWeapon.offCD() && m_iEnergy > m_pWeapon.getCost() + 1)
	{
		m_pWeapon.fire(m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2);
		m_iEnergy -= m_pWeapon.getCost();
	}
}
