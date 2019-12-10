/*---------------------------------------- GAME1007_M3_Bacon -------------------------------------------
							  GAME1007 Milesone 3, project by Kiera Bacon

Credits:
Background images: Free Cartoon Forest 2D Backgrounds by http://www.craftpix.net.
Player character and enemy images: Forest Monsters by Calciumtrice, usable under Creative Commons Attribution 3.0 license, https://opengameart.org/content/forest-monsters.
Player bullet image: modified from Animated Charged Bolt by Hansjörg Malthaner, http://opengameart.org/users/varkalandar.
Enemy bullet image: Animated Fires by Stealthix, usable under the Creative Commons Zero v1.0 Universal asset license, https://stealthix.itch.io/animated-fires.
HUD image (not used... yet): FANTASY GUI by MELLE, usable as part of the CC0 1.0 Universal Public Domain Dedication, https://opengameart.org/content/fantasy-gui-0.
//----------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------
		 Preprocessor and Global Variables
--------------------------------------------------*/
// Preprocessor directives
#include <algorithm>				// Used for bullet vector cleanup algorithm
#include <iostream>					// Used for printing debug info to the console
#include <vector>					// Used for saving a dynamic number of bullets
#include <ctime>					// Used for intitializing the random seed
#include <SDL.h>					// Base game engine
#include <SDL_image.h>				// Used for loading and rendering images
#include "Background.h"				// Class for the background
#include "Player.h"					// Class for the player
#include "ObjectSpawner.h"			// Class for the object spawner
#include "HUD.h"					// Class for the HUD
#include "Camera.h"					// Class for the camera

// Defined engine parameters
#define SCREENW 1024				// Width of the window in pixels
#define SCREENH 768					// Height of the window in pixels
#define FPS 60						// Number of frames per second of real time

// Defined gameplay parameters
#define IMG_BACKGROUND "Cartoon_Forest_BG_01.png"
#define IMG_ENERGYBAR "GUI_1.png"
#define IMG_BUTTERFLY "butterfly_spritesheet.png"
#define IMG_POLLEN "hjm-charged_bolt.png"
#define IMG_FIREBALL "Fireball_68x9.png"
#define IMG_EYE "eyeball_spritesheet.png"
#define WORLDW 1024					// Width of the game world in pixels == width of the screen
#define WORLDH 1080					// Height of the game world in pixels == height of the background image
#define BACKGROUNDFRAMES 2			// Number of frames per pixel moved by the background
#define PLAYERSPEED 5				// Number of pixels moved each frame by the player
#define PLAYERENERGYMAX 100			// Maximum amount of energy
#define PLAYERENERGY PLAYERENERGYMAX// Current amount of energy
#define PLAYERREGENRATE 3			// Number of frames per energy regen tick
#define PLAYERREGENQTY 1			// Amount of energy regenerated per tick
#define WEAPONSPEED 1				// Number of pixels moved each frame by the projectiles of the default weapon
#define WEAPONDMG 20				// Amount of damage dealt by the projectiles of the default weapon
#define WEAPONRFR 8					// Number of frames per shot fired by the default weapon
#define WEAPONENC 10				// Energy cost of each shot fired by the detault weapon

using namespace std;

// Global engine variables
bool g_bRunning = false;			// Loop control flag
Uint32 g_start, g_end, g_delta, g_fps;	// Fixed timestamp variables
const Uint8* g_iKeystates;			// Keyboard state container
SDL_Window* g_pWindow;				// This represents the SDL window
SDL_Renderer* g_pRenderer;			// This represents the buffer to draw to

// Global gameplay variables
int g_iBounds = 8;					// Bounds at the edge of the play area
Background g_background;			// Background object
SDL_Rect g_world;					// Frame inside which all gameplay happens
Camera g_camera;					// Camera object
Player g_player;					// Player object
HUD g_HUD;							// HUD object
Weapon g_weapon_Pollen1;			// Default weapon object
Weapon g_weapon_Fireball1;			// Default weapon object
ObjectSpawner g_objSpawner;			// Object spawner for the level
SDL_Texture* g_pTex_BG;				// Pointer to the background texture
SDL_Texture* g_pTex_EnergyBar;		// Pointer to the Energy bar texture
SDL_Texture* g_pTex_Butterfly;		// Pointer to the Butterfly texture
SDL_Texture* g_pTex_Pollen;			// Pointer to the Pollen texture
SDL_Texture* g_pTex_Fireball;		// Pointer to the Fireball texture
SDL_Texture* g_pTex_Eye;			// Pointer to the Eye texture
vector<Projectile*> g_pVec_AllyPrj;	// Holds pointers to dynamic Bullet objects, tested for collision against enemies.
vector<Enemy*> g_pVec_Enemy;		// Holds pointers to dynamic Enemy objects, tested for collision against the player.
vector<Projectile*> g_pVec_EnemyPrj;// Holds pointers to dynamic Bullet objects, tested for collision against the player.

// Global object templates
Sprite g_sPrj_Pollen;				// Template Sprite object for the player's default weapon projectiles
Sprite g_sMOB_Eye;					// Template Sprite object for the basic enemy
Enemy g_enemy_Eye;					// Template Enemy object for the basic enemy
Sprite g_sPrj_Fireball;				// Template Sprite object for the basic enemy's weapon projectiles

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
					g_pTex_BG = IMG_LoadTexture(g_pRenderer, IMG_BACKGROUND);
					g_pTex_EnergyBar = IMG_LoadTexture(g_pRenderer, IMG_ENERGYBAR);
					g_pTex_Butterfly = IMG_LoadTexture(g_pRenderer, IMG_BUTTERFLY);
					g_pTex_Pollen = IMG_LoadTexture(g_pRenderer, IMG_POLLEN);
					g_pTex_Fireball = IMG_LoadTexture(g_pRenderer, IMG_FIREBALL);
					g_pTex_Eye = IMG_LoadTexture(g_pRenderer, IMG_EYE);
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

	// Initialize static class members
	ObjectSpawner::setWorld(&g_world);
	MOB::setWorld(&g_world);

	// Initialize template objects
	g_sMOB_Eye = { 0, 32, 32, 32, 0, 0, 48, 48, g_pTex_Eye, 8, 2 };
	g_sPrj_Fireball = { 0, 0, 68, 9, 0, 0, 68, 9, g_pTex_Fireball, 10, 1 };
	g_weapon_Fireball1 = { &g_pVec_EnemyPrj, &g_sPrj_Fireball, beam, 3, 180, 0, 40, 1, 180, 0 };
	g_enemy_Eye = { g_sMOB_Eye, 0, 0, 1, 180, g_weapon_Fireball1, 30, 30, -1, 0 };
	
	g_sPrj_Pollen = { 0, 0, 32, 32, 0, 0, 16, 16, g_pTex_Pollen, 2, 4 };
	g_weapon_Pollen1 = { &g_pVec_AllyPrj, &g_sPrj_Pollen, beam, WEAPONSPEED, 0, 0, WEAPONDMG, 1, WEAPONRFR, WEAPONENC };

	// Initialize gameplay objects
	g_background = { BACKGROUNDFRAMES, 1, 0, 0, 1920, 1080, 0, 0, 1920, 1080, g_pTex_BG, 0, 0 };
	g_world = { 0, 0, WORLDW, WORLDH };
	g_player = { &g_weapon_Pollen1, PLAYERSPEED, PLAYERENERGY, PLAYERENERGYMAX, PLAYERREGENRATE, PLAYERREGENQTY,
		0, 0, 32, 32, g_world.w / 10, g_world.h / 2 - 16, 32, 32, g_pTex_Butterfly, 8, 1 };
	g_camera = { &g_player, &g_world, 0, g_player.getY() + g_player.getH() / 2 - SCREENH / 2, SCREENW, SCREENH };
	g_HUD = { &g_player, SCREENW, SCREENH };
	g_objSpawner = { &g_pVec_Enemy, &g_enemy_Eye, 240 };

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
	// Take input from the player and update the player's position
	if (keyDown(SDL_SCANCODE_W) && g_player.getY() - g_iBounds > 0)
		g_player.setY(g_player.getY() - g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_S) && g_player.getY() + g_player.getH() + g_iBounds < g_world.h)
		g_player.setY(g_player.getY() + g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_A) && g_player.getX() - g_iBounds > 0)
		g_player.setX(g_player.getX() - g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_D) && g_player.getX() + g_player.getW() + g_iBounds < g_world.w / 2)
		g_player.setX(g_player.getX() + g_player.getSpeed());
	if (keyDown(SDL_SCANCODE_SPACE))
		g_player.fire();

	// Update objects
	g_background.update(SCREENH, g_player.getY());
	g_camera.update();
	g_HUD.update();
	g_objSpawner.update();
	g_player.update();
	g_player.animate();
	g_weapon_Pollen1.update();
	g_weapon_Fireball1.update();

	// Update the vector of ally projectiles
	for (unsigned int i = 0; i < (int)g_pVec_AllyPrj.size(); i++)
	{
		g_pVec_AllyPrj[i]->update();

		// Check for collision against enemies
		for (unsigned int j = 0; j < g_pVec_Enemy.size(); j++)
		{
			if (SDL_HasIntersection(g_pVec_AllyPrj[i]->getDst(), g_pVec_Enemy[j]->getDst()))
			{
				g_pVec_AllyPrj[i]->setActive(false);
				g_pVec_Enemy[j]->setEnergy(-g_pVec_AllyPrj[i]->getDamage());
			}
		}

		// Update the vector
		if (!g_pVec_AllyPrj[i]->getActive())
		{
			delete g_pVec_AllyPrj[i];
			g_pVec_AllyPrj[i] = nullptr;
		}
		else
			g_pVec_AllyPrj[i]->animate();
	}
	if (!g_pVec_AllyPrj.empty())
	{
		g_pVec_AllyPrj.erase(remove(g_pVec_AllyPrj.begin(), g_pVec_AllyPrj.end(), nullptr), g_pVec_AllyPrj.end());
		g_pVec_AllyPrj.shrink_to_fit();
	}

	// Update the vector of enemy projectiles
	for (unsigned int i = 0; i < (int)g_pVec_EnemyPrj.size(); i++)
	{
		g_pVec_EnemyPrj[i]->update();

		// Check for collision against the playe
		if (SDL_HasIntersection(g_pVec_EnemyPrj[i]->getDst(), g_player.getDst()))
		{
			g_pVec_EnemyPrj[i]->setActive(false);
			g_player.setEnergy(-g_pVec_EnemyPrj[i]->getDamage());
		}

		// Update the vector
		if (!g_pVec_EnemyPrj[i]->getActive())
		{
			delete g_pVec_EnemyPrj[i];
			g_pVec_EnemyPrj[i] = nullptr;
		}
		else
			g_pVec_EnemyPrj[i]->animate();
	}
	if (!g_pVec_EnemyPrj.empty())
	{
		g_pVec_EnemyPrj.erase(remove(g_pVec_EnemyPrj.begin(), g_pVec_EnemyPrj.end(), nullptr), g_pVec_EnemyPrj.end());
		g_pVec_EnemyPrj.shrink_to_fit();
	}

	// Update the vector of enemies
	for (unsigned int i = 0; i < (int)g_pVec_Enemy.size(); i++)
	{
		g_pVec_Enemy[i]->update();
		if (!g_pVec_Enemy[i]->getActive())
		{
			delete g_pVec_Enemy[i];
			g_pVec_Enemy[i] = nullptr;
		}
		else
			g_pVec_Enemy[i]->animate();
	}
	if (!g_pVec_Enemy.empty())
	{
		g_pVec_Enemy.erase(remove(g_pVec_Enemy.begin(), g_pVec_Enemy.end(), nullptr), g_pVec_Enemy.end());
		g_pVec_Enemy.shrink_to_fit();
	}
}

// Screen render function
void render()
{
	// Render the background
	SDL_RenderCopy(g_pRenderer, g_background.getTex(), g_background.getSrc(), g_camera.getOffset(g_background.getDst()));
	SDL_RenderCopy(g_pRenderer, g_background.getTex(), g_background.getSrc(), g_camera.getOffset(g_background.getDst2()));
	
	// Render the HUD
	SDL_SetRenderDrawColor(g_pRenderer, 128, 144, 160, 64);
	SDL_RenderFillRect(g_pRenderer, g_HUD.getMax());
	SDL_SetRenderDrawColor(g_pRenderer, 176, 208, 240, 255);
	SDL_RenderFillRect(g_pRenderer, g_HUD.getCur());

	// Render the player
	SDL_RenderCopyEx(g_pRenderer, g_player.getTex(), g_player.getSrc(), g_camera.getOffset(g_player.getDst()), 0, nullptr, SDL_FLIP_HORIZONTAL);
	
	// Render the enemies
	for (unsigned int i = 0; i < (int)g_pVec_Enemy.size(); i++)
		SDL_RenderCopy(g_pRenderer, g_pVec_Enemy[i]->getTex(), g_pVec_Enemy[i]->getSrc(), g_camera.getOffset(g_pVec_Enemy[i]->getDst()));

	// Render the projectiles
	for (unsigned int i = 0; i < (int)g_pVec_AllyPrj.size(); i++)
		SDL_RenderCopy(g_pRenderer, g_pVec_AllyPrj[i]->getTex(), g_pVec_AllyPrj[i]->getSrc(), g_camera.getOffset(g_pVec_AllyPrj[i]->getDst()));
	for (unsigned int i = 0; i < (int)g_pVec_EnemyPrj.size(); i++)
		SDL_RenderCopy(g_pRenderer, g_pVec_EnemyPrj[i]->getTex(), g_pVec_EnemyPrj[i]->getSrc(), g_camera.getOffset(g_pVec_EnemyPrj[i]->getDst()));

	// Draw anew
	SDL_RenderPresent(g_pRenderer);
}

// Cleanup function
void shutdown()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyTexture(g_pTex_BG);
	SDL_DestroyTexture(g_pTex_Butterfly);
	SDL_DestroyTexture(g_pTex_Pollen);
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}

// Main function
int main(int argc, char* args[]) // Main MUST have these parameters for SDL.
{
	srand((unsigned)time(NULL));

	if (init("GAME1007_SDL_Setup", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREENW, SCREENH, 0) == false)
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