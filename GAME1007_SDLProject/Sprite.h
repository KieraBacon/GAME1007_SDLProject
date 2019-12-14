#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
class Sprite
{
protected:
	bool m_bActive = true;	// Flag to destroy the object if set to false
	bool m_bExpires = false;// Flag to set the object's active bool to false after animating once
	SDL_Rect m_dst;			// Destination rect for the sprite
	SDL_Rect m_src;			// Source rect for the texture
	SDL_Texture* m_pTex;	// Pointer to the texture
	int m_iSpriteMax;		// Final sprite in animation
	int m_iRowMax = 0;		// Number of rows in the texture file for a single animation
	int m_iFrameMax;		// Frame on which animation occurs
	int m_iSprite = 0;		// Current sprite counter
	int m_iRow = 0;			// Current row counter
	int m_iFrame = 0;		// Current frame counter
public:
	// Default constructor
	Sprite() {};

	// Base constructor that takes values for all members
	Sprite(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex, int sm = 1, int fm = -1, int rm = 1, bool e = false);

	// Copy constructor that takes x and y coordinates for the destination rectangle
	Sprite(const Sprite& base, int x, int y);

	// Accessor methods
	bool getActive() { return m_bActive; }
	int getX() { return m_dst.x; }
	int getY() { return m_dst.y; }
	int getW() { return m_dst.w; }
	int getH() { return m_dst.h; }
	SDL_Rect* getSrc() { return &m_src; }
	SDL_Rect* getDst() { return &m_dst; }
	SDL_Texture* getTex() { return m_pTex; }
	
	// Mutator methods
	void setActive(bool b) { m_bActive = b; }
	void setX(int x) { m_dst.x = x; }
	void setY(int y) { m_dst.y = y; }
	void setW(int w) { m_dst.w = w; }
	void setH(int h) { m_dst.h = h; }

	// Unique class methods
	void animate();
	virtual void update();
};





class Background :
	public Sprite
{
	static SDL_Rect* m_pWorld;	// Pointer to the world
	static SDL_Rect* m_pCamera;	// Pointer to the camera
	SDL_Rect m_dst2;			// Destination rectangle for the background looping object
	int m_iMoveCD;				// Number of frames since the last time the background moved
	int m_iMoveRate;			// Number of frames between each time the background moves
	int m_iSpeed;				// Number of pixels moved each time the background moves
public:
	// Default construcot
	Background() {};

	// Base constructor that assigns all members
	Background(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex, int sm, int fm, int mr, int spd = 1);

	// Accessor methods
	SDL_Rect* getDst2() { return &m_dst2; }

	// Static mutators
	static void setWorld(SDL_Rect* w) { m_pWorld = w; }
	static void setCamera(SDL_Rect* c) { m_pCamera = c; }

	// Unique class methods
	void update();
};





class MOB : 
	public Sprite
{
protected:
	static SDL_Rect* m_pWorld;	// Pointer to the world
	SDL_Rect m_col;				// Collision rect for the MOB
	SDL_Rect m_col_dst;			// Processed collision rect for the MOB
	Mix_Chunk* m_pSnd_End;		// Sound played on impact with another collidable
	int m_iMoveCD;				// Number of frames since the last time the enemy moved
	int m_iMoveRate;			// Number of frames between each time the enemy moves
	int m_iSpeed;				// Speed of movement in pixels per move
	float m_fAngle;				// Angle from the x axis
public:
	// Default constructor
	MOB() {};

	// Constructor that takes an object of the base class, plus x and y spawn coordinates
	MOB(const Sprite& base, int x, int y, SDL_Rect col, Mix_Chunk* esnd, int mr, int spd, float ang);

	// Constructor that takes values for all parameters
	MOB(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm, Mix_Chunk* esnd, int mr, int spd, float ang);

	// Accessor methods
	SDL_Rect* getCol();
	Mix_Chunk* getSnd_End() { return m_pSnd_End; }
	int getMR() { return m_iMoveRate; }
	int getSpeed() { return m_iSpeed; }
	float getAngle() { return m_fAngle; }

	// Mutator methods
	void setCol(SDL_Rect col) { m_col = col; }
	void setSnd_End(Mix_Chunk* snd) { m_pSnd_End = snd; }
	void setSpeed(int s) { m_iSpeed = s; }
	void setMR(int mr) { m_iMoveRate = mr; }
	void setAngle(float a) { m_fAngle = a; }

	// Static mutator
	static void setWorld(SDL_Rect* w) { m_pWorld = w; }

	// Unique class methods
	void update();
};





class Projectile :
	public MOB
{
	Mix_Chunk* m_pSnd_Start;	// Sound played when the projectile is launched
	int m_iDamage;				// Damage dealt upon collision
public:
	// Default constructor
	Projectile() {};

	// Constructor that takes values for all members
	Projectile(SDL_Rect src, SDL_Rect dst, SDL_Rect col, SDL_Texture* tex, int sm, int fm,
		Mix_Chunk* ssnd, Mix_Chunk* esnd, int mr = 1, int spd = 1, float ang = 0, int dmg = 0);

	// Copy constructor that takes x and y spawn coordinates, speed, angle, and damage
	Projectile(const Projectile& base, int x, int y, int mr, int spd, float ang, int dmg);

	// Accessor methods
	int getDamage() { return m_iDamage; }
	Mix_Chunk* getSnd_Start() { return m_pSnd_Start; }
};