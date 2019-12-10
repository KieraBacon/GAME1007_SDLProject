#pragma once
#include <SDL.h>
#include <SDL_image.h>
class Sprite
{
protected:
	SDL_Rect m_dst;		// Destination rect for the sprite
	SDL_Rect m_src;		// Source rect for the texture
	SDL_Texture* m_tex;	// Pointer to the texture
	int m_spriteMax;	// Final sprite in animation
	int m_frameMax;		// Frame on which animation occurs
	int m_sprite = 0;	// Current sprite counter
	int m_frame = 0;	// Current frame counter
public:
	// Default constructor
	Sprite() {};

	// Base constructor that assigns all members
	Sprite(int src_x, int src_y, int src_w, int src_h,
		int dst_x, int dst_y, int dst_w, int dst_h,
		SDL_Texture* tex, int spriteMax, int frameMax);

	// Minimal constructor assumes certain values
	Sprite(int x, int y, int w, int h,
		SDL_Texture* tex, int spriteMax, int frameMax);

	// Copy constructor that takes x and y coordinates
	Sprite(const Sprite& base, int x, int y);

	// Accessor methods
	int getX() { return m_dst.x; }
	int getY() { return m_dst.y; }
	int getW() { return m_dst.w; }
	int getH() { return m_dst.h; }
	SDL_Rect* getSrc() { return &m_src; }
	SDL_Rect* getDst() { return &m_dst; }
	SDL_Texture* getTex() { return m_tex; }
	
	// Mutator methods
	void setX(int x) { m_dst.x = x; }
	void setY(int y) { m_dst.y = y; }
	void setW(int w) { m_dst.w = w; }
	void setH(int h) { m_dst.h = h; }

	// Unique class methods
	void animate();
};