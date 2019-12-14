/*---------------------------------------- GAME1007_M4_Bacon -------------------------------------------
							  GAME1007 Milesone 4, project by Kiera Bacon

Credits:
Background images: "Free Cartoon Forest 2D Backgrounds" by http://www.craftpix.net.
Player character and enemy images: "Forest Monsters" by Calciumtrice, used under the Creative Commons Attribution 3.0 license, https://opengameart.org/content/forest-monsters.
Player bullet image: modified from "Animated Charged Bolt" by Hansjörg Malthaner, http://opengameart.org/users/varkalandar.
Enemy bullet image: "Animated Fires" by Stealthix, used under the Creative Commons Zero v1.0 Universal asset license, https://stealthix.itch.io/animated-fires.
HUD image (not used... yet): "FANTASY GUI" by MELLE, used as part of the CC0 1.0 Universal Public Domain Dedication, https://opengameart.org/content/fantasy-gui-0.
Branch image: "Forest themed sprites" © 2012-2013 Julien Jorge <julien.jorge@stuff-o-matic.com>, retrieved from https://opengameart.org/content/forest-themed-sprites.
Explosion image: "25 special effects rendered with blender" by rubberduck, used as part of the CC0 1.0 Universal Public Domain Dedication, https://opengameart.org/content/25-special-effects-rendered-with-blender.
Player and Enemy death sound: "Ghost pain" by Vinrax, used under the Creative Commons Attribution 3.0 license, https://opengameart.org/content/ghost-pain .
Player bullet start sound: "4 dry snow steps" by qubodup, used as part of the CC0 1.0 Universal Public Domain Dedication, https://opengameart.org/content/4-dry-snow-steps.
Player bullet end sound: "Swish - bamboo stick weapon swhoshes" by Iwan Gabovitch, used as part of the CC0 1.0 Universal Public Domain Dedication, https://opengameart.org/content/swish-bamboo-stick-weapon-swhoshes.
Enemy bullet start sound: "Flap, Splat, Poo (Yo Frankie!)" by Blender Foundation, used under the Creative Commons Attribution 3.0 license, https://opengameart.org/content/flap-splat-poo-yo-frankie.
Enemy bullet end sound: "Fantozzi's Footsteps (Grass/Sand & Stone)", used as part of the CC0 1.0 Universal Public Domain Dedication,  https://opengameart.org/content/fantozzis-footsteps-grasssand-stone.
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
#include <SDL_mixer.h>				// Used for managing sounds and music
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
#define IMG_BRANCH "forest-1.png"
#define IMG_FLAMERING "ring_blueish_flame.png"
#define SND_MUSIC "Cleyton RX - Underwater.mp3"
#define SND_FIREBALL_START "bird_flap.flac"
#define SND_FIREBALL_END "Fantozzi-SandR1.flac"
#define SND_POLLEN_START "snow_step_dry-04.flac"
#define SND_POLLEN_END "swosh-13.flac"
#define SND_EYE_END "ghost.flac"
#define WORLDW 1024					// Width of the game world in pixels == width of the screen
#define WORLDH 1080					// Height of the game world in pixels == height of the background image
#define BACKGROUNDFRAMES 2			// Number of frames per pixel moved by the background
#define PLAYERSPEED 5				// Number of pixels moved each frame by the player
#define PLAYERENERGYMAX 100			// Maximum amount of energy
#define PLAYERREGENRATE 3			// Number of frames per energy regen tick
#define OBSTACLEGRIDSIZE 64			// Size of the obstacles, and the obstacle grid, in pixels 

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
ObjectSpawner<Enemy> g_objSpawner_Enemy1;		// Object spawner for Enemies
ObjectSpawner<Enemy> g_objSpawner_Obstacle1;	// Object spawner for obtacles
vector<Projectile*> g_pVec_AllyPrj;	// Holds pointers to dynamic Projectile objects, tested for collision against enemies.
vector<Enemy*> g_pVec_Enemy;		// Holds pointers to dynamic Enemy objects, tested for collision against the player.
vector<Projectile*> g_pVec_EnemyPrj;// Holds pointers to dynamic Projectile objects, tested for collision against the player.
vector<Sprite*> g_pVec_Effect;		// Holds pointers to dynamic Sprite objects that expire on their own and do not collide with anything

// Global asset pointers
SDL_Texture* g_pTex_BG;				// Pointer to the background texture
SDL_Texture* g_pTex_EnergyBar;		// Pointer to the Energy bar texture
SDL_Texture* g_pTex_Butterfly;		// Pointer to the Butterfly texture
SDL_Texture* g_pTex_Pollen;			// Pointer to the Pollen texture
SDL_Texture* g_pTex_Fireball;		// Pointer to the Fireball texture
SDL_Texture* g_pTex_Eye;			// Pointer to the Eye texture
SDL_Texture* g_pTex_Branch;			// Pointer to the Branch texture
SDL_Texture* g_pTex_FlameRing;		// Pointer to the Ring of Fire texture
Mix_Chunk* g_pSnd_Pollen_Start;		// Pointer to the Pollen start sound
Mix_Chunk* g_pSnd_Pollen_End;		// Pointer to the Pollen end sound
Mix_Chunk* g_pSnd_Fireball_Start;	// Pointer to the Fireball start sound
Mix_Chunk* g_pSnd_Fireball_End;		// Pointer to the Fireball end sound
Mix_Chunk* g_pSnd_Eye_End;			// Pointer to the Eye end sound
Mix_Music* g_pMusic;				// Pointer to the music

// Global object templates
Sprite g_sEffect_FlameRing;			// Template Sprite for the flame ring effect
Sprite g_sUI_EnergyBar;				// Template Sprite for the UI Energy Bars

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
					g_pTex_Branch = IMG_LoadTexture(g_pRenderer, IMG_BRANCH);
					g_pTex_FlameRing = IMG_LoadTexture(g_pRenderer, IMG_FLAMERING);
				}
				else return false; // Texture init fail

				// Load the mixer
				if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC) != 0) // Mixer init success.
				{
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 8192);
					Mix_AllocateChannels(16);
					g_pMusic = Mix_LoadMUS(SND_MUSIC);
					g_pSnd_Pollen_Start = Mix_LoadWAV(SND_POLLEN_START);
					g_pSnd_Pollen_End = Mix_LoadWAV(SND_POLLEN_END);
					g_pSnd_Fireball_Start = Mix_LoadWAV(SND_FIREBALL_START);
					g_pSnd_Fireball_End = Mix_LoadWAV(SND_FIREBALL_END);
					g_pSnd_Eye_End = Mix_LoadWAV(SND_EYE_END);
					Mix_Volume(-1, MIX_MAX_VOLUME); // Volume for all channels.
					Mix_VolumeMusic(MIX_MAX_VOLUME / 2); // Volume for channel 0 to 64.
				}
				else return false;
			}
			else return false; // Renderer init fail
		}
		else return false; // Window init fail
	}
	else return false; // SDL init fail

	g_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds
	g_iKeystates = SDL_GetKeyboardState(nullptr);

	// Initialize static class members
	ObjectSpawner<Enemy>::setWorld(&g_world);
	MOB::setWorld(&g_world);

	// Create template objects

	// Flame Ring effect
	g_sEffect_FlameRing = { SDL_Rect{ 0, 0, 128, 128 }, SDL_Rect{ 0, 0, 64, 64 }, g_pTex_FlameRing, 8, 1, 5, true };

	// Pollen shooter weapon
	Projectile l_prj_Pollen = { SDL_Rect{ 0, 0, 32, 32 }, SDL_Rect{ 0, 0, 16, 16 }, SDL_Rect{ 1, 1, 14, 14 },
		g_pTex_Pollen, 2, 4, g_pSnd_Pollen_Start, g_pSnd_Pollen_End };
	Weapon l_wpn_Pollen1 = { &g_pVec_AllyPrj, l_prj_Pollen, beam, 1, 1, 0, 20, 0, 1, 8, 10 };

	// Fireball shooter weapon
	Projectile l_prj_Fireball = { SDL_Rect{ 0, 0, 68, 9 }, SDL_Rect{ 0, 0, 68, 9 }, SDL_Rect{ 0, 0, 9, 9 },
		g_pTex_Fireball, 10, 1, g_pSnd_Fireball_Start, g_pSnd_Fireball_End };
	Weapon l_wpn_Fireball1 = { &g_pVec_EnemyPrj, l_prj_Fireball, beam, 1, 2, 0, 40, 0, 1, 180, 0 };

	// Floating eyeball enemy
	Enemy l_enemy_Eye = {
		MOB{ SDL_Rect{ 0, 0, 32, 32 }, SDL_Rect{ 0, 0, 48, 48 }, SDL_Rect{ 8, 8, 32, 32 }, g_pTex_Eye, 8, 2, g_pSnd_Eye_End, 1, 1, 180 },
		EnergyPool{ 30, 30, -1, 0 },
		Weapon{ l_wpn_Fireball1, &g_pVec_EnemyPrj, 180 } };

	// Branch obstacle (/enemy)
	Enemy l_enemy_Branch = {
		MOB{ SDL_Rect{ 800, 0, 200, 100 }, SDL_Rect{ 0, 0, OBSTACLEGRIDSIZE, OBSTACLEGRIDSIZE }, SDL_Rect{ 5, 5, 54, 54 }, g_pTex_Branch, 1, -1, g_pSnd_Fireball_End, BACKGROUNDFRAMES, 1, 180 },
		EnergyPool{ -1, -1, -1, -1 },
		Weapon{ nullptr, Projectile{}, beam, 0, 0, 0, 0, 0, 0, -1, 0 } };

	// Initialize gameplay objects
	g_background = { SDL_Rect{ 0, 0, 1920, 1080 }, SDL_Rect{ 0, 0, 1920, 1080 }, g_pTex_BG, 1, -1, BACKGROUNDFRAMES };
	g_world = { 0, 0, WORLDW, WORLDH };
	g_player = { SDL_Rect{ 0, 0, 32, 32 }, SDL_Rect{g_world.w / 10, g_world.h / 2 - 16, 32, 32 }, SDL_Rect{ 1, 1, 30, 30},
		g_pTex_Butterfly, 8, 2, g_pSnd_Eye_End, l_wpn_Pollen1, PLAYERSPEED, PLAYERENERGYMAX, PLAYERENERGYMAX, PLAYERREGENRATE, 1 };
	g_camera = { &g_player, &g_world, 0, g_player.getY() + g_player.getH() / 2 - SCREENH / 2, SCREENW, SCREENH };
	g_HUD = { &g_player, SCREENW, SCREENH };
	g_objSpawner_Enemy1 = { &g_pVec_Enemy, l_enemy_Eye, 0, 2, 240, 1 };
	g_objSpawner_Obstacle1 = { &g_pVec_Enemy, l_enemy_Branch, 0, 5, OBSTACLEGRIDSIZE * BACKGROUNDFRAMES, OBSTACLEGRIDSIZE };

	Mix_PlayMusic(g_pMusic, -1);

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

// Vector handling function overload for ally projectiles -> enemies
void updateVector(vector<Projectile*>& pvec, vector<Enemy*>& evec)
{
	for (unsigned int i = 0; i < (int)pvec.size(); i++)
	{
		// Update all elements in the vector
		pvec[i]->update();

		// Check all elements in the vector for collision against the target vector
		for (unsigned int j = 0; j < evec.size(); j++)
		{
			if (SDL_HasIntersection(pvec[i]->getCol(), evec[j]->getCol()))
			{
				// Flag the projectile to be deleted and take away the enemy's energy
				pvec[i]->setActive(false);
				evec[j]->setEnergy(-pvec[i]->getDamage());

				// Spawn the effect
				g_pVec_Effect.push_back(new Sprite(g_sEffect_FlameRing,
					pvec[i]->getX() + (pvec[i]->getW() / 2) - (g_sEffect_FlameRing.getW() / 2),
					pvec[i]->getY() + (pvec[i]->getH() / 2) - (g_sEffect_FlameRing.getH() / 2)));

				// Play the collision sound
				int channel = Mix_PlayChannel(-1, pvec[i]->getSnd_End(), 0);
				Mix_Volume(channel, 128);
			}
		}

		// Update the vector
		if (!pvec[i]->getActive())
		{
			delete pvec[i];
			pvec[i] = nullptr;
		}
		else
			pvec[i]->animate();
	}
	if (!pvec.empty())
	{
		pvec.erase(remove(pvec.begin(), pvec.end(), nullptr), pvec.end());
		pvec.shrink_to_fit();
	}
}

// Vector handling function overload for enemy projectiles -> player
void updateVector(vector<Projectile*>& pvec, Player& player)
{
	for (unsigned int i = 0; i < (int)pvec.size(); i++)
	{
		pvec[i]->update();

		// Check for collision against the player
		if (player.getActive() == true && SDL_HasIntersection(pvec[i]->getCol(), player.getCol()))
		{
			// Flag the projectile to be deleted and take away the player's energy
			pvec[i]->setActive(false);
			player.setEnergy(-pvec[i]->getDamage());

			// Spawn the effect
			g_pVec_Effect.push_back(new Sprite(g_sEffect_FlameRing,
				pvec[i]->getX() + (pvec[i]->getW() / 2) - (g_sEffect_FlameRing.getW() / 2),
				pvec[i]->getY() + (pvec[i]->getH() / 2) - (g_sEffect_FlameRing.getH() / 2)));

			// Play the collision sound
			int channel = Mix_PlayChannel(-1, pvec[i]->getSnd_End(), 0);
			Mix_Volume(channel, 128);
		}

		// Update the vector
		if (!pvec[i]->getActive())
		{
			delete pvec[i];
			pvec[i] = nullptr;
		}
		else
			pvec[i]->animate();
	}
	if (!pvec.empty())
	{
		pvec.erase(remove(pvec.begin(), pvec.end(), nullptr), pvec.end());
		pvec.shrink_to_fit();
	}
}

// Vector handling function overload for enemies -> player
void updateVector(vector<Enemy*>& vec, Player& player)
{
	for (unsigned int i = 0; i < (int)vec.size(); i++)
	{
		vec[i]->update();

		// Check for collision against the player
		if (player.getActive() == true && SDL_HasIntersection(vec[i]->getCol(), player.getCol()))
		{
			// Calculate the amount of energy being taken away
			int energyCost = vec[i]->getEnergyPool()->getEnergy();		// Base damage on collision = the enemy's energy total
			SDL_Rect* deathSpot = vec[i]->getDst();
			if (energyCost == -1 || energyCost >= player.getEnergy())	// If the enemy has infinite energy, or just more than the player
			{
				energyCost = player.getEnergy();						// The damage is = the player's energy total
				deathSpot = player.getDst();
			}

			// Take away the calculated energy from both the player and the enemy
			player.setEnergy(-energyCost);							// Subtract the energy cost from the player's pool
			vec[i]->setEnergy(-energyCost);					// ... and the enemy's pool. Only one of them can survive the encounter.

			// Spawn the effect
			g_pVec_Effect.push_back(new Sprite(g_sEffect_FlameRing,
				deathSpot->x + (deathSpot->w / 2) - (g_sEffect_FlameRing.getW() / 2),
				deathSpot->y + (deathSpot->h / 2) - (g_sEffect_FlameRing.getH() / 2)));

			// Play the collision sound
			int channel = Mix_PlayChannel(-1, vec[i]->getSnd_End(), 0);
			Mix_Volume(channel, 128);
		}

		// Update the UI
		if (vec[i]->getEnergyPool()->getEnergy() < vec[i]->getEnergyPool()->getEnergyMax())
		{
			// Have a vector of Energy Bars, create a new Energy Bar each time if it isn't able to find one there already.
			// Have an Energy Bar member of each enemy.
			// OK, what if the Energy Bar member was of the Energy Pool class?
		}

		// Update the vector
		if (!vec[i]->getActive())
		{
			delete vec[i];
			vec[i] = nullptr;
		}
		else
			vec[i]->animate();
	}
	if (!vec.empty())
	{
		vec.erase(remove(vec.begin(), vec.end(), nullptr), vec.end());
		vec.shrink_to_fit();
	}
}

// Vector handling function overload for effects
void updateVector(vector<Sprite*>& vec)
{
	for (unsigned int i = 0; i < (int)vec.size(); i++)
	{
		// Update the vector
		if (!vec[i]->getActive())
		{
			delete vec[i];
			vec[i] = nullptr;
		}
		else
			vec[i]->animate();
	}
	if (!vec.empty())
	{
		vec.erase(remove(vec.begin(), vec.end(), nullptr), vec.end());
		vec.shrink_to_fit();
	}
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
	g_background.update();
	g_camera.update();
	g_HUD.update();
	g_objSpawner_Enemy1.update();
	g_objSpawner_Obstacle1.update();
	g_player.update();

	//Update vectors
	updateVector(g_pVec_AllyPrj, g_pVec_Enemy);
	updateVector(g_pVec_EnemyPrj, g_player);
	updateVector(g_pVec_Enemy, g_player);
	updateVector(g_pVec_Effect);
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

	// Render the Objects
	for (unsigned int i = 0; i < (int)g_pVec_Enemy.size(); i++)		// Enemies
		SDL_RenderCopy(g_pRenderer, g_pVec_Enemy[i]->getTex(), g_pVec_Enemy[i]->getSrc(), g_camera.getOffset(g_pVec_Enemy[i]->getDst()));
	for (unsigned int i = 0; i < (int)g_pVec_AllyPrj.size(); i++)	// Ally Projectiles
		SDL_RenderCopy(g_pRenderer, g_pVec_AllyPrj[i]->getTex(), g_pVec_AllyPrj[i]->getSrc(), g_camera.getOffset(g_pVec_AllyPrj[i]->getDst()));
	for (unsigned int i = 0; i < (int)g_pVec_EnemyPrj.size(); i++)	// Enemy Projectiles
		SDL_RenderCopy(g_pRenderer, g_pVec_EnemyPrj[i]->getTex(), g_pVec_EnemyPrj[i]->getSrc(), g_camera.getOffset(g_pVec_EnemyPrj[i]->getDst()));
	for (unsigned int i = 0; i < (int)g_pVec_Effect.size(); i++)	// Special Effects
		SDL_RenderCopy(g_pRenderer, g_pVec_Effect[i]->getTex(), g_pVec_Effect[i]->getSrc(), g_camera.getOffset(g_pVec_Effect[i]->getDst()));

	// Draw anew
	SDL_RenderPresent(g_pRenderer);
}

// Cleanup function
void shutdown()
{
	cout << "Cleaning game." << endl;
	g_pVec_EnemyPrj.clear();			// Empty all elements.
	g_pVec_EnemyPrj.shrink_to_fit();	// Shrink down capacity to 0.
	g_pVec_Enemy.clear();				// Empty all elements.
	g_pVec_Enemy.shrink_to_fit();		// Shrink down capacity to 0.
	g_pVec_AllyPrj.clear();				// Empty all elements.
	g_pVec_AllyPrj.shrink_to_fit();		// Shrink down capacity to 0.
	Mix_CloseAudio();
	Mix_FreeChunk(g_pSnd_Fireball_End);
	Mix_FreeChunk(g_pSnd_Fireball_Start);
	Mix_FreeChunk(g_pSnd_Pollen_End);
	Mix_FreeChunk(g_pSnd_Pollen_Start);
	Mix_FreeMusic(g_pMusic);
	SDL_DestroyTexture(g_pTex_Branch);
	SDL_DestroyTexture(g_pTex_Eye);
	SDL_DestroyTexture(g_pTex_Fireball);
	SDL_DestroyTexture(g_pTex_Pollen);
	SDL_DestroyTexture(g_pTex_Butterfly);
	SDL_DestroyTexture(g_pTex_EnergyBar);
	SDL_DestroyTexture(g_pTex_BG);
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}

// Main function
int main(int argc, char* args[]) // Main MUST have these parameters for SDL.
{
	srand((unsigned)time(NULL));

	if (init("GAME1007_M4_Bacon", SDL_WINDOWPOS_CENTERED,
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