#include "Player.h"

void Player::update()
{
	if (m_iRegenCD == m_iRegenRate && m_iEnergy < m_iEnergyMax)
	{
		m_iRegenCD = 0;
		m_iEnergy + m_iRegenQty >= m_iEnergyMax ? m_iEnergy = m_iEnergyMax : m_iEnergy += m_iRegenQty;
	}
	else
		m_iRegenCD++;
}