// ----------------------------------------------------------------
// Quick Side Scroller (https://github.com/d0n3val/Quick-Side-Scroller)
// Simplistic side scroller made with SDL for educational purposes.
//
// Installation
// Project files are made for VS 2015. Download the code, compile it. There is no formal installation process.
//
// Credits
// Ricard Pillosu
//
// License
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain.We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors.We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
// 
// ----------------------------------------------------------------

#include "SDL\include\SDL.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "SDL\include\SDL_ttf.h"
#include<stdlib.h>
#include<time.h>

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )
#pragma comment( lib, "SDL/libx86/SDL2_ttf.lib")

//Primer de tot recordar que el ric no es mirara el codi

// Globals --------------------------------------------------------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCROLL_SPEED 0
#define SCROLL_SPEED2 1
#define SCROLL_SPEED3 2
#define SCROLL_SPEED4 4
#define SCROLL_SPEED5 5
#define SCROLL_SPEED6 6
#define SHIP_SPEED 5
#define NUM_SHOTS 32
#define SHOT_SPEED 5
#define NUM_ENEMIES 80
#define NUM_TANKS 80
#define ENEMY_TEXTURE_WIDTH 165
#define TANK_TEXTURE_WIDTH 80
#define ENEMY_SPEED 5
#define ENEMY_SPEEDT 4
#define COLLISION_MARGIN 10
#define COLLISION_MARGIN_Y 30
#define COLLISION_MARGIN_SX 40
#define COLLISION_MARGIN_SY 20
#define TANK_COLLISION_MARGIN 50

struct projectile
{
	int x, y;
	bool alive;
	SDL_Rect src = { 0,0,11,8 };
};

struct enemy 
{
	int x, y;
	bool alive;
	bool explosion;
	bool ex_sound;
	SDL_Rect src;
}e;

struct tank
{
	int x, y;
	bool alive;
	bool explosion;
	bool ex_sound;
	SDL_Rect src;
};

struct globals
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect Explosion;
	SDL_Texture* background = nullptr;
	SDL_Texture* background2 = nullptr;
	SDL_Texture* background3 = nullptr;
	SDL_Texture* background4 = nullptr;
	SDL_Texture* background5 = nullptr;
	SDL_Texture* background6 = nullptr;
	SDL_Texture* gameover = nullptr;
	SDL_Texture* ship = nullptr;
	SDL_Texture* shot = nullptr;
	SDL_Texture* enemy_texture = nullptr;
	SDL_Texture* Sprites = nullptr;
	SDL_Texture* tanks_texture = nullptr;
	int background_width = 640;
	int ship_x = 0;
	int ship_y = 0;
	int last_shot = 0;
	bool fire, up, down, left, right;
	bool retu;
	Mix_Music* music = nullptr;
	Mix_Chunk* fx_shoot = nullptr;
	Mix_Chunk* exp = nullptr;
	int scroll = 0;
	int scroll2 = 0;
	int scroll3 = 0;
	int scroll4 = 0;
	int scroll5 = 0;
	int scroll6 = 0;
	projectile shots[NUM_SHOTS];
	enemy enemies[NUM_ENEMIES];
	tank tanks[NUM_TANKS];
} g; // automatically create an instance called "g"

// ----------------------------------------------------------------

void Start()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Renderer *renderer = nullptr;
	
	// Create window & renderer
	g.window = SDL_CreateWindow("QSS - Quick Side Scroller by APERTURE WOLVES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	//initial page
	


	// Load image lib --
	IMG_Init(IMG_INIT_PNG);
	g.background = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/parallax-mountain-bg.png"));
	g.background2 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/background2.png"));
	g.background3= SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/background3.png"));
	g.background4 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/background4.png"));
	g.background5 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/background5.png"));
	g.background6 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/background6.png"));
	g.ship = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/ship.png"));
	g.shot = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/shot.png"));
	g.enemy_texture = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/ships.png"));
	SDL_QueryTexture(g.background, nullptr, nullptr, &g.background_width, nullptr);
	g.Sprites = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/sprites.png"));
	g.tanks_texture = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/tank.png"));

	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);
	SDL_Color White = { 0, 0, 0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect Message_rect = { 0, 0, 100, 100 };
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	// Create mixer --
	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	g.music = Mix_LoadMUS("assets/music.ogg");
	Mix_PlayMusic(g.music, -1);
	g.fx_shoot = Mix_LoadWAV("assets/laser.wav");
	g.exp = Mix_LoadWAV("assets/exp.wav");

	// Init other vars --
	g.ship_x = 100;
	g.ship_y = SCREEN_HEIGHT / 2;
	g.fire = g.up = g.down = g.left = g.right = false;

	// Init enemies -p
	srand(time(NULL)); //Random seed
	int fila;
	int col;
	for (int i = 0; i < NUM_ENEMIES; i++) {
		g.enemies[i].x = SCREEN_WIDTH + 50 + (400 * i) ;
		g.enemies[i].y = (rand()%400)-20; //Random position 
		g.enemies[i].alive = true;
		g.enemies[i].explosion = true;
		g.enemies[i].ex_sound = false;
		fila = rand() % 3;
		col = rand() % 3;
		g.enemies[i].src = { ENEMY_TEXTURE_WIDTH*fila, 56*col, ENEMY_TEXTURE_WIDTH, 56};
	}

	//Init tanks	
	for (int i = 0; i < NUM_TANKS; ++i) {
		g.tanks[i].x = SCREEN_WIDTH + 50 + (500 * i);
		g.tanks[i].y = 390;
		g.tanks[i].alive = true;
		g.tanks[i].explosion = true;
		g.tanks[i].ex_sound = false;		
		g.tanks[i].src = { 0, 0, TANK_TEXTURE_WIDTH, 66 };
	}

	g.retu = true;
	
}

// ----------------------------------------------------------------
void Finish()
{
	Mix_FreeMusic(g.music);
	Mix_FreeChunk(g.fx_shoot);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyTexture(g.shot);
	SDL_DestroyTexture(g.ship);
	SDL_DestroyTexture(g.background);
	SDL_DestroyTexture(g.Sprites);
	IMG_Quit();
	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_Quit();
}

// ----------------------------------------------------------------
bool CheckInput()
{
	bool ret = true;
	SDL_Event event;

	while(SDL_PollEvent(&event) != 0)
	{
		if(event.type == SDL_KEYUP)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_UP: g.up = false;	break;
				case SDLK_DOWN:	g.down = false;	break;
				case SDLK_LEFT:	g.left = false;	break;
				case SDLK_RIGHT: g.right = false; break;
			}
		}
		else if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_UP: g.up = true; break;
				case SDLK_DOWN: g.down = true; break;
				case SDLK_LEFT: g.left = true; break;
				case SDLK_RIGHT: g.right = true; break;
				case SDLK_ESCAPE: ret = false; break;
				case SDLK_SPACE: g.fire = (event.key.repeat == 0); break;
			}
		}
		//else if (event.type == SDL_QUIT)
			//ret = false;
	}

	return ret;
}

void collisionDetection()  
{
	for (int enemy_index = 0; enemy_index < NUM_ENEMIES; enemy_index++) {
		for (int shot_index = 0; shot_index < NUM_SHOTS; shot_index++) {
			if (g.shots[shot_index].alive == true && g.enemies[enemy_index].alive == true) {
				if ((g.shots[shot_index].x + COLLISION_MARGIN) > g.enemies[enemy_index].x ) {
					if ((g.shots[shot_index].x - COLLISION_MARGIN) < g.enemies[enemy_index].x) {
						if ((g.shots[shot_index].y + COLLISION_MARGIN) > g.enemies[enemy_index].y) {
							if ((g.shots[shot_index].y - COLLISION_MARGIN_Y) < g.enemies[enemy_index].y) {
								g.enemies[enemy_index].alive = false;
								g.shots[shot_index].alive = false;
								g.enemies[enemy_index].explosion = false;
								g.enemies[enemy_index].ex_sound = true;							
							}
						}
					}
				}
			}
		}
	}

	for (int enemy_index = 0; enemy_index < NUM_TANKS; enemy_index++) {
		for (int shot_index = 0; shot_index < NUM_SHOTS; shot_index++) {
			if (g.shots[shot_index].alive == true && g.tanks[enemy_index].alive == true) {
				if ((g.shots[shot_index].x + COLLISION_MARGIN) > g.tanks[enemy_index].x) {
					if ((g.shots[shot_index].x - COLLISION_MARGIN) < g.tanks[enemy_index].x) {
						if ((g.shots[shot_index].y + COLLISION_MARGIN) > g.tanks[enemy_index].y) {
							if ((g.shots[shot_index].y - TANK_COLLISION_MARGIN) < g.tanks[enemy_index].y) {
								g.tanks[enemy_index].alive = false;
								g.shots[shot_index].alive = false;
								g.tanks[enemy_index].explosion = false;
								g.tanks[enemy_index].ex_sound = true;
							}
						}
					}
				}
			}
		}
	}
	
	for (int enemy_index = 0; enemy_index < NUM_TANKS; enemy_index++) {
			if (g.tanks[enemy_index].alive == true) {
				if ( g.ship_x+ COLLISION_MARGIN_SX > g.enemies[enemy_index].x) {
					if ((g.ship_x - COLLISION_MARGIN_SX) < g.enemies[enemy_index].x) {
						if ((g.ship_y + COLLISION_MARGIN_SY) > g.enemies[enemy_index].y) {
							if ((g.ship_y - COLLISION_MARGIN_SY) < g.enemies[enemy_index].y) {
								g.retu = false;
							}
						}
					}
				}
			}
		}
		
}

// ----------------------------------------------------------------
void MoveStuff()
{
	// Calc new ship position
	
	if (g.up)	//143x46		//640x480
	{
		if (g.ship_y > 0)
			g.ship_y -= SHIP_SPEED;
		else
			g.ship_y = 0;
	}

	if (g.down)
	{
		if (g.ship_y < 434)
			g.ship_y += SHIP_SPEED;
		else
			g.ship_y = 434;
	}
		
	if (g.left)
	{
		if (g.ship_x > 0)
			g.ship_x -= SHIP_SPEED;
		else
			g.ship_x = 0;
	}

	if (g.right)
	{
		if (g.ship_x < 540)
			g.ship_x += SHIP_SPEED;
		else
			g.ship_x = 540;
	}
	if(g.fire)
	{
		Mix_PlayChannel(-1, g.fx_shoot, 0);
		g.fire = false;

		if(g.last_shot == NUM_SHOTS)
			g.last_shot = 0;

		g.shots[g.last_shot].alive = true;
		g.shots[g.last_shot].x = g.ship_x + 92;
		g.shots[g.last_shot].y = g.ship_y + 23;
		++g.last_shot;
	}

	for(int i = 0; i < NUM_SHOTS; ++i)
	{
		if(g.shots[i].alive)
		{
			if(g.shots[i].x < SCREEN_WIDTH)
				g.shots[i].x += SHOT_SPEED;
		}
	}

	//enemies
	for (int i = 0; i < NUM_ENEMIES; ++i)
	{
		if (g.enemies[i].alive)
		{
			if (1)
				g.enemies[i].x -= ENEMY_SPEED;
			else
				g.enemies[i].alive = false;
		}
	}
	//tanks
	for (int i = 0; i < NUM_TANKS; ++i)
	{
		if (g.tanks[i].alive)
		{
			if (1)
				g.tanks[i].x -= ENEMY_SPEEDT;
			else
				g.tanks[i].alive = false;
		}
	}
}

// ----------------------------------------------------------------
void Draw()
{
	SDL_Rect target;

	target = { 0, 0, 640, 370 };
	SDL_RenderCopy(g.renderer, g.background, nullptr, &target);

	// Scroll and draw background 2

	g.scroll2 += SCROLL_SPEED2;
	if(g.scroll2 >= 700)
	g.scroll2 = 0;

	target = { -g.scroll2, -100, 700, SCREEN_HEIGHT };

	SDL_RenderCopy(g.renderer, g.background2, nullptr, &target);
	target.x += 700;
	SDL_RenderCopy(g.renderer, g.background2, nullptr, &target);

	// Scroll and draw background 3

	g.scroll3 += SCROLL_SPEED3;
	if (g.scroll3 >= 1000)
		g.scroll3 = 0;

	target = { -g.scroll3, 0, 1000, SCREEN_HEIGHT };

	SDL_RenderCopy(g.renderer, g.background3, nullptr, &target);
	target.x += 1000;
	SDL_RenderCopy(g.renderer, g.background3, nullptr, &target);

	// Scroll and draw background 3

	g.scroll4 += SCROLL_SPEED4;
	if (g.scroll4 >= 1000)
		g.scroll4 = 0;

	target = { -g.scroll4, -0, 1000, SCREEN_HEIGHT };

	SDL_RenderCopy(g.renderer, g.background4, nullptr, &target);
	target.x += 1000;
	SDL_RenderCopy(g.renderer, g.background4, nullptr, &target);


	

	// Draw player's ship --
	target = { g.ship_x, g.ship_y, 100, 40 };
	SDL_RenderCopy(g.renderer, g.ship, nullptr, &target);

	// Draw lasers --
	for(int i = 0; i < NUM_SHOTS; ++i)
	{
		if(g.shots[i].alive)
		{
			target = { g.shots[i].x, g.shots[i].y, 11, 8 };
			SDL_RenderCopy(g.renderer, g.shot, &g.shots[i].src, &target);
		}
	}

	//Draw enemies -p
	for (int i = 0; i < NUM_ENEMIES; ++i)
	{
		if (g.enemies[i].alive)
		{
			target = { g.enemies[i].x, g.enemies[i].y, 88, 35 };
			SDL_RenderCopy(g.renderer, g.enemy_texture, &g.enemies[i].src, &target);
		}
	}

	//Draw enemies -p
	for (int i = 0; i < NUM_TANKS; ++i)
	{
		if (g.tanks[i].alive)
		{
			target = { g.tanks[i].x, g.tanks[i].y, 80, 66 };
			SDL_RenderCopy(g.renderer, g.tanks_texture, &g.tanks[i].src, &target);
		}
	}

	// Scroll and draw background 4

	g.scroll5 += SCROLL_SPEED5;
	if (g.scroll5 >= 700)
		g.scroll5 = 0;

	target = { -g.scroll5, 85, 700, SCREEN_HEIGHT };

	SDL_RenderCopy(g.renderer, g.background5, nullptr, &target);
	target.x += 700;
	SDL_RenderCopy(g.renderer, g.background5, nullptr, &target);

	// Scroll and draw background 5

	g.scroll6 += SCROLL_SPEED6;
	if (g.scroll6 >= 700)
		g.scroll6 = 0;

	target = { -g.scroll6, 0, 700, SCREEN_HEIGHT };

	SDL_RenderCopy(g.renderer, g.background6, nullptr, &target);
	target.x += 700;
	SDL_RenderCopy(g.renderer, g.background6, nullptr, &target);

	//Draw explosion
	for (int i = 0; i < NUM_ENEMIES; ++i)
	{
		g.Explosion = { 224,224,32,32 };
		if (g.enemies[i].explosion == false)
		{
			Mix_PlayChannel(-1, g.exp, 0);
			target = { g.enemies[i].x, g.enemies[i].y, 64, 64 };
			SDL_RenderCopy(g.renderer, g.Sprites, &g.Explosion, &target);
			g.enemies[i].explosion = true;
		}
	}

	for (int i = 0; i < NUM_TANKS; ++i)
	{
		g.Explosion = { 224,224,32,32 };
		if (g.tanks[i].explosion == false)
		{
			Mix_PlayChannel(-1, g.exp, 0);
			target = { g.tanks[i].x, g.tanks[i].y, 64, 64 };
			SDL_RenderCopy(g.renderer, g.Sprites, &g.Explosion, &target);
			g.tanks[i].explosion = true;
		}
	}

	// Finally swap buffers
	SDL_RenderPresent(g.renderer);
}

// ----------------------------------------------------------------
int main(int argc, char* args[])
{
	
	Start();
	IMG_Init(IMG_INIT_PNG);
	SDL_Rect SSC;
	SDL_Texture* StaSc;
	SDL_Texture* GO;
	SSC = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	StaSc = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/init.png"));
	GO = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("assets/go.png"));
	SDL_RenderCopy(g.renderer, StaSc, NULL, &SSC);
	SDL_RenderPresent(g.renderer);
	SDL_Delay(3000);
	
	

	while(CheckInput() && g.retu)
	{
		collisionDetection();
		MoveStuff();
		Draw();
	}
	
	SDL_RenderCopy(g.renderer, GO, NULL, &SSC);
	SDL_RenderPresent(g.renderer);
	SDL_Delay(3000);
	
	Finish();


	return(0); // EXIT_SUCCESS
}