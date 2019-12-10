#include "Weapon.h"

Weapon::Weapon(std::vector<Projectile*>* vec, Sprite* spr, pattern pat,
	int spd, float ang, int off, int dmg, int qty, int rfr, int enc) :
	m_pPrjVec(vec),
	m_pPrjSprite(spr),
	m_ePrjPattern(pat),
	m_iPrjSpeed(spd),
	m_fPrjAngle(ang),
	m_iPrjOff(off),
	m_iPrjDamage(dmg),
	m_iPrjQty(qty),
	m_iPrjNum(1),
	m_iRefireCD(rfr),
	m_iRefireRate(rfr),
	m_iEnergyCost(enc) {}

Weapon::Weapon(const Weapon& base, float ang) :
	m_pPrjVec(base.m_pPrjVec),
	m_pPrjSprite(base.m_pPrjSprite),
	m_ePrjPattern(base.m_ePrjPattern),
	m_iPrjSpeed(base.m_iPrjSpeed),
	m_fPrjAngle(base.m_fPrjAngle + ang),
	m_iPrjOff(base.m_iPrjOff),
	m_iPrjDamage(base.m_iPrjDamage),
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
	if (m_iPrjNum == 1)
	{
		m_iRefireCD = 0;
		m_pPrjVec->push_back(new Projectile(*m_pPrjSprite, x, y, m_iPrjSpeed, m_fPrjAngle, m_iPrjDamage));
	}
	else if (m_ePrjPattern == beam)
	{
		for (int i = m_iPrjNum % 2; i < m_iPrjNum; i++)
		{
			m_iRefireCD = 0;
			m_pPrjVec->push_back(new Projectile(*m_pPrjSprite, x, y + m_iPrjOff * i * (2 * (i % 2) - 1), m_iPrjSpeed, m_fPrjAngle, m_iPrjDamage));
		}
	}
	else if (m_ePrjPattern == spray)
	{
		for (int i = m_iPrjNum % 2; i < m_iPrjNum; i++)
		{
			m_iRefireCD = 0;
			m_pPrjVec->push_back(new Projectile(*m_pPrjSprite, x, y, m_iPrjSpeed, m_fPrjAngle * i * (2 * (i % 2) - 1), m_iPrjDamage));
		}
	}
	else if (m_ePrjPattern == alternate)
	{
		m_iPrjNum == 1 ? m_iPrjNum = -1 : m_iPrjNum = 1;
		m_iRefireCD = 0;
		m_pPrjVec->push_back(new Projectile(*m_pPrjSprite, x, y + m_iPrjOff * m_iPrjNum, m_iPrjSpeed, m_fPrjAngle, m_iPrjDamage));
	}
}