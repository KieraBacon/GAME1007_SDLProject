#include "Weapon.h"

Weapon::Weapon(std::vector<Projectile*>* vec, const Projectile& prj, pattern pat, int spd, float ang, int dmg, int off, int qty, int rfr, int enc) :
	m_pPrjVec(vec),
	m_prj(prj),
	m_ePattern(pat),
	m_iPrjMR(0),
	m_iPrjSpeed(spd),
	m_fPrjAngle(ang),
	m_iDamage(dmg),
	m_iPrjOff(off),
	m_iPrjQty(qty),
	m_iPrjNum(1),
	m_iRefireCD(rfr),
	m_iRefireRate(rfr),
	m_iEnergyCost(enc) {}

Weapon::Weapon(const Weapon& base, std::vector<Projectile*>* vec, float ang) :
	m_pPrjVec(vec),
	m_prj(base.m_prj),
	m_ePattern(base.m_ePattern),
	m_iPrjMR(base.m_iPrjMR),
	m_iPrjSpeed(base.m_iPrjSpeed),
	m_fPrjAngle(ang),
	m_iDamage(base.m_iDamage),
	m_iPrjOff(base.m_iPrjOff),
	m_iPrjQty(base.m_iPrjQty),
	m_iPrjNum(base.m_iPrjNum),
	m_iRefireCD(base.m_iRefireCD),
	m_iRefireRate(base.m_iRefireRate),
	m_iEnergyCost(base.m_iEnergyCost) {}

bool Weapon::offCD()
{
	if (m_iRefireCD >= m_iRefireRate)
		return true;
	else
		return false;
}

void Weapon::update()
{
	if (m_iRefireCD < m_iRefireRate)
		m_iRefireCD++;
}

void Weapon::fire(int x, int y)
{
	if (m_iPrjNum > 0)
	{
		if (m_iPrjNum == 1)
		{
			m_iRefireCD = 0;
			m_pPrjVec->push_back(new Projectile(m_prj, x, y, m_iPrjMR, m_iPrjSpeed, m_fPrjAngle, m_iDamage));
		}
		else if (m_ePattern == beam)
		{
			for (int i = m_iPrjNum % 2; i < m_iPrjNum; i++)
			{
				m_iRefireCD = 0;
				m_pPrjVec->push_back(new Projectile(m_prj, x, y + m_iPrjOff * i * (2 * (i % 2) - 1), m_iPrjMR, m_iPrjSpeed, m_fPrjAngle, m_iDamage));
			}
		}
		else if (m_ePattern == spray)
		{
			for (int i = m_iPrjNum % 2; i < m_iPrjNum; i++)
			{
				m_iRefireCD = 0;
				m_pPrjVec->push_back(new Projectile(m_prj, x, y, m_iPrjMR, m_iPrjSpeed, m_fPrjAngle * i * (2 * (i % 2) - 1), m_iDamage));
			}
		}
		else if (m_ePattern == alternate)
		{
			m_iPrjNum == 1 ? m_iPrjNum = -1 : m_iPrjNum = 1;
			m_iRefireCD = 0;
			m_pPrjVec->push_back(new Projectile(m_prj, x, y + m_iPrjOff * m_iPrjNum, m_iPrjMR, m_iPrjSpeed, m_fPrjAngle, m_iDamage));
		}

		int channel = Mix_PlayChannel(-1, m_prj.getSnd_Start(), 0);
		if (channel != -1)
			Mix_Volume(channel, 64);
	}
}