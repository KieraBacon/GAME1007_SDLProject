#pragma once
#include "Sprite.h"
class MOB :
	public Sprite
{
protected:
	static SDL_Rect* m_pWorld;	// Pointer to the world
	bool m_bActive = true;		// Flag to destroy the object if set to false
	int m_iSpeed;				// Speed of movement in pixels per frame
	float m_fAngle;				// Angle from the x axis
public:
	// Default constructor
	MOB() {};

	// Constructor that takes an object of the base class, plus x and y spawn coordinates
	MOB(const Sprite& base, int x, int y, int spd, float ang);

	// Constructor that takes values for all parameters
	MOB(int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h,
		SDL_Texture* tex, int spriteMax, int frameMax,
		int spd, float ang);

	// Accessor methods
	bool getActive() { return m_bActive; }

	// Mutator methods
	void setActive(bool b) { m_bActive = b; }

	// Static mutator
	static void setWorld(SDL_Rect* w) { m_pWorld = w; }

	// Unique class methods
	void update();
	
};