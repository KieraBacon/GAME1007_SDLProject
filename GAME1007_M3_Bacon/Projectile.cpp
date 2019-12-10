#include "Projectile.h"
#include <cmath>		// Used for sin and cos functions
#define PI 3.14159265

Projectile::Projectile(const Sprite& base, int x, int y, int spd, float ang, int dmg) :
	MOB(base, x, y, spd, ang),
	m_iDamage(dmg) {}

Projectile::Projectile(int src_x, int src_y, int src_w, int src_h,
	int dst_x, int dst_y, int dst_w, int dst_h,
	SDL_Texture * tex, int spriteMax, int frameMax,
	int spd, float ang,
	int dmg) :
	MOB(src_x, src_y, src_w, src_h,
		dst_x, dst_y, dst_w, dst_h, tex,
		spriteMax, frameMax, spd, ang),
	m_iDamage(dmg) {}