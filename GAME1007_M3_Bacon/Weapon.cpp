#include "Weapon.h"

int Weapon::fire(int x, int y)
{
	if (m_iRefireCD == m_iRefireRate)
	{
		if (m_iPrjNum == 1)
		{
			m_iRefireCD = 0;
			m_pPrjVec->push_back(new Projectile(m_pPrjSprite, x, y, m_iPrjSpeed, m_fPrjAngle, m_iPrjDamage));
		}
		else if (m_ePrjPattern == beam)
		{
			for (int i = m_iPrjNum % 2; i < m_iPrjNum; i++)
			{
				m_iRefireCD = 0;
				m_pPrjVec->push_back(new Projectile(m_pPrjSprite, x, y + m_iPrjOff * i * (2 * (i % 2) - 1), m_iPrjSpeed, m_fPrjAngle, m_iPrjDamage));
			}
		}
		else if (m_ePrjPattern == spray)
		{
			for (int i = m_iPrjNum % 2; i < m_iPrjNum; i++)
			{
				m_iRefireCD = 0;
				m_pPrjVec->push_back(new Projectile(m_pPrjSprite, x, y, m_iPrjSpeed, m_fPrjAngle * i * (2 * (i % 2) - 1), m_iPrjDamage));
			}
		}
		else if (m_ePrjPattern == alternate)
		{
			m_iPrjNum == 1 ? m_iPrjNum = -1 : m_iPrjNum = 1;
			m_iRefireCD = 0;
			m_pPrjVec->push_back(new Projectile(m_pPrjSprite, x, y + m_iPrjOff * m_iPrjNum, m_iPrjSpeed, m_fPrjAngle, m_iPrjDamage));
		}
		return -1;
	}
	else
		m_iRefireCD++;
	return 0;
}
