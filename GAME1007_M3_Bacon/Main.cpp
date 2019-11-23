/*---------------------------------------- GAME1007_M2_Bacon -------------------------------------------
							  GAME1007 Milesone 2, project by Kiera Bacon

Credits:
Background images: Free Cartoon Forest 2D Backgrounds by http://www.craftpix.net.
Player character image: Forest Monsters by Calciumtrice, usable under Creative Commons Attribution 3.0 license, https://opengameart.org/content/forest-monsters.
Player bullet image: modified from Animated Charged Bolt by Hansjörg Malthaner, http://opengameart.org/users/varkalandar.
//----------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------
		 Preprocessor and Global Variables
--------------------------------------------------*/
// Preprocessor directives
#include <algorithm>	// Used for bullet vector cleanup algorithm
#include <iostream>		// Used for printing debug info to the console
#include <vector>		// Used for saving a dynamic number of bullets
#include <SDL.h>		// Base game engine
#include <SDL_image.h>	// Used for loading and rendering images
#include "Background.h"	// Class for the background
#include "Player.h"		// Class for the player

#define SPRITE_BACKGROUND "Cartoon_Forest_BG_01.png"
#define SPRITE_PLAYER "butterfly spritesheet.png"
#define SPRITE_BULLET "hjm-charged_bolt.png"
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

#define PLAYERSPEED 5				// Speed of the player's movement
#define PLAYERENERGYMAX 100			// Maximum amount of energy
#define PLAYERENERGY PLAYERENERGYMAX// Current amount of energy
#define PLAYERREGENRATE 5			// Number of frames between regen ticks
#define PLAYERREGENQTY 1			// Amount of energy regenerated per tick
#define BACKGROUNDFRAMES 2			// Speed of the background
using namespace std;

// Global engine variables
bool g_bRunning = false;			// Loop control flag
Uint32 g_start, g_end, g_delta, g_fps;	// Fixed timestamp variables
const Uint8* g_iKeystates;			// Keyboard state container
SDL_Window* g_pWindow;				// This represents the SDL window
SDL_Renderer* g_pRenderer;			// This represents the buffer to draw to
SDL_Rect g_rfield;					// This represents the playing field

// Global gameplay variables
int g_iBounds = 8;					// Bounds at the edge of the play area
Player g_player;					// Player object
Sprite g_projectileSprite;			// Base sprite object for all projectiles
Weapon g_weapon;					// Base weapon object
Background g_Background;			// Background object
SDL_Rect g_rMaxAmmo, g_rCurAmmo;	// Ammo display objects.
SDL_Texture* g_pTex_BG;				// Pointer to the background texture
SDL_Texture* g_pTex_Player;			// Pointer to the player texture
SDL_Texture* g_pTex_Bullet;			// Pointer to the bullet texture
vector<Projectile*> g_vProjectileVec;	// Holds pointers to dynamically-created Bullet objects.

/*--------------------------------------------------
			  Functions for game loop
--------------------------------------------------*/

// Initialization function
bool init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game." << endl;
	// Attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window
		g_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (g_pWindow != nullptr) // Window init success
		{
			// Create the renderer
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
			if (g_pRenderer != nullptr) // Renderer init success
			{
				// Load the textures
				if (IMG_Init(IMG_INIT_PNG)) {
					g_pTex_BG = IMG_LoadTexture(g_pRenderer, SPRITE_BACKGROUND);
					g_pTex_Player = IMG_LoadTexture(g_pRenderer, SPRITE_PLAYER);
					g_pTex_Bullet = IMG_LoadTexture(g_pRenderer, SPRITE_BULLET);
				}
				else return false; // Texture init fail
			}
			else return false; // Renderer init fail
		}
		else return false; // Window init fail
	}
	else return false; // SDL init fail

	g_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds
	g_iKeystates = SDL_GetKeyboardState(nullptr);
	g_rfield = { 0, 0, WIDTH, HEIGHT };

	// Create the base gameplay objects
	g_projectileSprite = { 0, 0, 32, 32, 0, 0, 16, 16, g_pTex_Bullet, 2, 4 };
	g_weapon = { &g_vProjectileVec, &g_projectileSprite, beam, 5, 0, 0, 1, 1, 8 };
	g_player = { &g_weapon, PLAYERSPEED, PLAYERENERGY, PLAYERENERGYMAX, PLAYERREGENRATE, PLAYERREGENQTY, 
		0, 0, 32, 32, WIDTH / 10, HEIGHT / 2 - 16, 32, 32, g_pTex_Player, 8, 1 };
	g_Background = { BACKGROUNDFRAMES, 1, 0, 0, 1920, 1080, 0, 0, 1920, 1080, g_pTex_BG };

	// Create the ammo sprite
	//g_rMaxAmmo = { WIDTH / 100, HEIGHT / 100, WIDTH / 80, HEIGHT / 8 };
	//g_rCurAmmo = { g_rMaxAmmo.x, g_rMaxAmmo.y, g_rMaxAmmo.w, g_rMaxAmmo.h * static_cast<int>(g_dPlayerCurAmmo / g_dPlayerMaxAmmo) };

	// Everything is okay, start the engine
	g_bRunning = true;
	cout << "Success!" << endl;
	return true;
}

// Frame length initiation function
void wake()
{
	g_start = SDL_GetTicks();
}

// Frame length control function
void sleep()
{
	g_end = SDL_GetTicks();
	g_delta = g_end - g_start;
	if (g_delta < g_fps) // Engine has to sleep
		SDL_Delay(g_fps - g_delta);
}

// Event handling function
void handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			g_bRunning = false;
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				g_bRunning = false;
			break;
		}
	}
}

// Keyboard utility function
bool keyDown(SDL_Scancode c)
{
	if (g_iKeystates != nullptr)
	{
		if (g_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

// Gamestate update function
void update()
{
	// This is the main game stuff
	//bool l_bAnimate = false; // Flag for whether to animate
	//if (g_dPlayerFireCD > 0)
	//	g_dPlayerFireCD -= dt; // Update the refire cooldown
	//if (g_dPlayerCurAmmo != g_dPlayerMaxAmmo)
	//{
	//	if (g_dPlayerCurAmmo + dt * g_dPlayerAmmoRegen <= g_dPlayerMaxAmmo)
	//		g_dPlayerCurAmmo += dt * g_dPlayerAmmoRegen; // Update the player's ammo
	//	else
	//		g_dPlayerCurAmmo = g_dPlayerMaxAmmo; // But only to the max
	//}
	//if (g_dAnimCD > 0)
	//	g_dAnimCD -= dt; // Update the reanimation cooldown
	//else
	//{
	//	g_dAnimCD = g_dAnimRate;
	//	l_bAnimate = true;
	//}

	// Take input from the player and update the player's position
	if (keyDown(SDL_SCANCODE_W) && g_player.getY() - g_iBounds > 0)
		g_player.setY(g_player.getY() - g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_S) && g_player.getY() + g_player.getH() + g_iBounds < HEIGHT)
		g_player.setY(g_player.getY() + g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_A) && g_player.getX() - g_iBounds > 0)
		g_player.setX(g_player.getX() - g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_D) && g_player.getX() + g_player.getW() + g_iBounds < WIDTH / 2)
		g_player.setX(g_player.getX() + g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_SPACE) && g_player.m_iEnergy > 0)
		g_player.m_iEnergy += g_player.m_pWeapon->fire(g_player.getX() + g_player.getW() / 2, g_player.getY() + g_player.getH() / 2);
	// Update objects
	g_Background.update(HEIGHT, g_player.getY());
	g_player.update();
	g_player.animate();
	for (int i = 0; i < (int)g_vProjectileVec.size(); i++)
	{
		g_vProjectileVec[i]->update(WIDTH, HEIGHT);
		if (!g_vProjectileVec[i]->getActive())
		{
			delete g_vProjectileVec[i];
			g_vProjectileVec[i] = nullptr;
		}
		else
			g_vProjectileVec[i]->animate();
	}
	if (!g_vProjectileVec.empty())
	{
		g_vProjectileVec.erase(remove(g_vProjectileVec.begin(), g_vProjectileVec.end(), nullptr), g_vProjectileVec.end());
		g_vProjectileVec.shrink_to_fit();
	}
}

// Screen render function
void render()
{
	// Render stuff
	SDL_RenderCopy(g_pRenderer, g_Background.getTex(), g_Background.getSrc(), g_Background.getDst());
	SDL_RenderCopy(g_pRenderer, g_Background.getTex(), g_Background.getSrc(), g_Background.getDst2());
	SDL_SetRenderDrawColor(g_pRenderer, 128, 144, 160, 64);
	SDL_RenderFillRect(g_pRenderer, &g_rMaxAmmo);
	SDL_SetRenderDrawColor(g_pRenderer, 176, 208, 240, 255);
	SDL_RenderFillRect(g_pRenderer, &g_rCurAmmo);
	SDL_RenderCopyEx(g_pRenderer, g_player.getTex(), g_player.getSrc(), g_player.getDst(), 0, nullptr, SDL_FLIP_HORIZONTAL);
	for (int i = 0; i < (int)g_vProjectileVec.size(); i++)
		SDL_RenderCopy(g_pRenderer, g_vProjectileVec[i]->getTex(), g_vProjectileVec[i]->getSrc(), g_vProjectileVec[i]->getDst());

	// Draw anew
	SDL_RenderPresent(g_pRenderer);
}

// Cleanup function
void shutdown()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyTexture(g_pTex_BG);
	SDL_DestroyTexture(g_pTex_Player);
	SDL_DestroyTexture(g_pTex_Bullet);
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}

// Main function
int main(int argc, char* args[]) // Main MUST have these parameters for SDL.
{
	if (init("GAME1007_SDL_Setup", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;

	while (g_bRunning)
	{
		// Represent the diffence in time between 2 frames in ms
		wake();
		handleEvents();
		update();
		render();
		if (g_bRunning)
			sleep();
	}

	shutdown();
	return 0;
}