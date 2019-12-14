#include "EnergyPool.h"



EnergyPool::EnergyPool(int ngy, int nmx, int rgr, int rgq) :
m_iEnergy(ngy), m_iEnergyMax(nmx), m_iRegenRate(rgr), m_iRegenQty(rgq) {}

void EnergyPool::setEnergy(int e)
{
	if (m_iEnergyMax != -1)
	{
		if (m_iEnergy + e >= m_iEnergyMax)
			m_iEnergy = m_iEnergyMax;
		else if (m_iEnergy + e <= 0)
		{
			m_iEnergy = 0;
		}
		else
			m_iEnergy += e;
	}
}

void EnergyPool::update()
{
	// Update energy
	if (m_iEnergyMax != -1)
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
	}
}