#include "HUD.h"

HUD::HUD(Player * p, int w, int h) :
	m_pPlayer(p),
	m_rMaxEnergy{ w / 100, h / 100, w / 80, h / 8 },
	m_rCurEnergy{ m_rMaxEnergy.x, m_rMaxEnergy.y, m_rMaxEnergy.w, m_rMaxEnergy.h * p->getEnergy() / p->getEnergyMax() } {};

int HUD::update()
{
	m_rCurEnergy.h = static_cast<int>(m_rMaxEnergy.h * m_pPlayer->getEnergy() / static_cast<double>(m_pPlayer->getEnergyMax()));
	if (m_rCurEnergy.h > m_rMaxEnergy.h * 2 / 3)
		return 0;
	else if (m_rCurEnergy.h > m_rMaxEnergy.h / 3)
		return 1;
	else
		return 2;
}