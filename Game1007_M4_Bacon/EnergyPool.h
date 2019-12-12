#pragma once
#include <SDL.h>
class EnergyPool
{
	int m_iEnergy;		// Current amount of energy
	int m_iEnergyMax;	// Maximum amount of energy
	int m_iRegenCD;		// Number of frames since last regen tick
	int m_iRegenRate;	// Number of frames between regen ticks
	int m_iRegenQty;	// Amount of energy regenerated per tick
public:
	// Default constructor
	EnergyPool() {};

	// Constructor that takes values for all members
	EnergyPool(int ngy, int nmx, int rgr, int rgq);

	// Accessor methods
	int getEnergy() { return m_iEnergy; };
	int getEnergyMax() { return m_iEnergyMax; };

	// Mutator methods
	void setEnergy(int e);

	// Unique class methods
	void update();
};

