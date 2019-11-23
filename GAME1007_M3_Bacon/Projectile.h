#pragma once
#include "Sprite.h"

class Projectile : public Sprite
{
private:
	bool m_bActive = true;	// Flag to destroy the object if set to false
	int m_iSpeed;			// Speed
	float m_fAngle;			// Angle from x axis
	int m_iDamage;			// Damage
public:
	// Default constructor
	Projectile() {};

	// Constructor that takes a pointer to a sprite object and x, y spawn coordinates as parameters
	Projectile(Sprite* base, int x, int y, int speed, float angle, int damage) :
		Sprite(base->m_src.x, base->m_src.y, base->m_src.w, base->m_src.h,
			x, y, base->m_dst.w, base->m_dst.h, base->m_tex,
			base->m_spriteMax, base->m_frameMax, base->m_sprite, base->m_frame),
		m_iSpeed(speed), m_fAngle(angle), m_iDamage(damage) {};

	// Constructor that requires values for all parameters
	Projectile(int speed, float angle, int damage,
		int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture* tex,
		int spriteMax = 0, int frameMax = 0, int sprite = 0, int frame = 0) :
		Sprite(src_x, src_y, src_w, src_h,
			dst_x, dst_y, dst_w, dst_h, tex,
			spriteMax, frameMax, sprite, frame),
		m_iSpeed(speed),
		m_fAngle(angle),
		m_iDamage(damage) {};

	void update(int xMax, int yMax);
	bool getActive() { return m_bActive; }
};