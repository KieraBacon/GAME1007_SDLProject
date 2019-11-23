#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Sprite
{
	friend class Projectile;
	//friend class Weapon;
protected:
	SDL_Rect m_dst;
	SDL_Rect m_src;
	SDL_Texture* m_tex;
	int m_spriteMax;
	int m_frameMax;
	int m_sprite;
	int m_frame;
public:
	// Default constructor
	Sprite() {};

	// Base constructor requires all values be defined
	Sprite(int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h, SDL_Texture* tex,
		int spriteMax = 0, int frameMax = 0, int sprite = 0, int frame = 0) :
		m_src{ src_x, src_y, src_w, src_h },
		m_dst{ dst_x, dst_y, dst_w, dst_h },
		m_tex(tex),
		m_spriteMax(spriteMax),
		m_frameMax(frameMax),
		m_sprite(sprite),
		m_frame(frame) {};

	// Minimal constructor assumes certain values
	Sprite(int x, int y, int w, int h, SDL_Texture* tex,
		int spriteMax = 0, int frameMax = 0, int sprite = 0, int frame = 0) :
		m_src{ 0, 0, w, h },
		m_dst{ x, y, w, h },
		m_tex(tex),
		m_spriteMax(spriteMax),
		m_frameMax(frameMax),
		m_sprite(sprite),
		m_frame(frame) {};

	// Accessor functions
	int getX() { return m_dst.x; };
	int getY() { return m_dst.y; };
	int getW() { return m_dst.w; };
	int getH() { return m_dst.h; };
	SDL_Rect* getSrc() { return &m_src; }
	SDL_Rect* getDst() { return &m_dst; }
	SDL_Texture* getTex() { return m_tex; }
	
	// Mutator functions
	void setX(int x) { m_dst.x = x; }
	void setY(int y) { m_dst.y = y; }
	void setW(int w) { m_dst.w = w; }
	void setH(int h) { m_dst.h = h; }

	// Class methods
	void animate();
	void renderOffset(SDL_Renderer * renderer, SDL_Rect* camera, int screenW, int screenH);
};