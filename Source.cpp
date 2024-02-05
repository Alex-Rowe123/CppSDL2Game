#include <SDL.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Map.h"
#include "Player.h"

constexpr int mapx = 45;
constexpr int mapy = 45;
void DisplayText(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, int _r, int _g, int _b, int _x, int _y);


int main(int argc, char* argv[])
{
	srand(time(NULL));
	// SDL SETUP /////////

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Error during init";
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("window", 100, 60, 900, 900, 0);
	if (!window){ std::cout << "Error with window"; }

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) { std::cout << "Error creating renderer"; }

	int test = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	if (test != 0)
	{
		std::cout << "Error creating audio device";
	}

	if (TTF_Init() < 0)
	{
		std::cout << "Error with ttf init" << TTF_GetError();
	}

	TTF_Font* font = TTF_OpenFont("Comme-ExtraBold.ttf", 32);
	if (!font) std::cout << "Font failed to load" << TTF_GetError();
	// SDL SETUP END //////

	bool gamerunning = false;
	bool shouldquit = false;
	auto keyboard = SDL_GetKeyboardState(NULL);
	while (shouldquit == false) // menu and app loop
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderClear(renderer);
		keyboard = SDL_GetKeyboardState(NULL); // Get state of keyboard
		if (keyboard[SDL_SCANCODE_ESCAPE]) shouldquit = true;
		if (keyboard[SDL_SCANCODE_RETURN]) gamerunning = true;

		DisplayText(renderer,font,"GOOP GAME", 255,255,255,50,200);
		DisplayText(renderer, font, "Press ENTER to start", 255, 255, 255, 50, 350);
		DisplayText(renderer, font, "Press ESC to quit", 255, 255, 255, 50, 400);

		DisplayText(renderer, font, "Controls:", 255, 255, 255, 50, 500);
		DisplayText(renderer, font, "WASD to move", 255, 255, 255, 50, 550);
		DisplayText(renderer, font, "Aim with mouse", 255, 255, 255, 50, 600);
		DisplayText(renderer, font, "LMB to shoot", 255, 255, 255, 50, 650);
		DisplayText(renderer, font, "BACKSPACE to go back to main menu", 255, 255, 255, 50, 700);

		SDL_RenderPresent(renderer);

		if (gamerunning) // ingame loop
		{
			// INITIALIZE VARIABLES /////////
			MapObj* map = new MapObj(renderer); // construct map object
			Player* player = new Player(100, Vector2D(450, 150), map, renderer, "test.bmp"); // construct player
			map->m_Player = player;
			// make map grid. true = wall, false = empty space
			bool MapGrid[30][30] =
			{
				{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{1,0,1,1,1,1,1,0,1,0,0,1,0,1,1,1,1,1,0,1},
				{1,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,1},
				{1,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1},
				{1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
				{1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
				{1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1},
				{1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
				{1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1},
				{1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1},
				{1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
				{1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1},
				{1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
				{1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
				{1,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1},
				{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
				{1,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,1},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			};
			// Apply above grid to the map object
			for (int i{ 0 }; i < 30 * 30; i++)
			{
				map->m_MapGrid[0][i] = MapGrid[0][i];
			}
			map->GenerateMap(); // spawn enemies

			Mix_AllocateChannels(20);

			// INITIALIZE VARIABLES END /////

			// // GAMEPLAY LOOP // //
			while (gamerunning)
			{
				// RENDERING //
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
				SDL_RenderClear(renderer);

				// create rectangle for walls
				SDL_Rect rect;
				rect.w = mapx;
				rect.h = mapy;
				rect.x = 0;
				rect.y = 0;

				// draw walls to screen loop
					//vertical loop
				for (int i{ 0 }; i < 20; i++)
				{
					rect.y = i * mapy; // move rectangle along
					rect.x = 0; // set its x to 0

					// horizontal loop
					for (int j{ 0 }; j < 20; j++)
					{
						rect.x = j * mapx; // move rectangle along
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
						if (MapGrid[i][j] == true) SDL_RenderFillRect(renderer, &rect); // if there is a wall draw a wall
					}
				}

				// GAME LOGIC //
				map->calculateDelta(); // calculate delta time
				player->Movement(); // apply movement to the player
				player->DoTick(); // do the players tick function
				map->EnemyDoTick(); // loop through all enemies and do their tick function
				map->BulletDoTick();

				// print health to screen
				std::string health = "Health: ";
				health.append(std::to_string(player->GetHealth()));
				DisplayText(renderer, font, health.data(), 0, 255, 0, 20, 20);
				// print score to screen
				std::string score = "Score: ";
				score.append(std::to_string(player->m_score));
				DisplayText(renderer, font, score.data(), 0, 255, 0, 20, 50);



				if (map->m_EnemyList.empty())
				{
					DisplayText(renderer, font, "YOU WIN!!", 0, 0, 255, 50, 100);
					DisplayText(renderer, font, "Press BACKSPACE to go back to the main menu", 0, 0, 255, 50, 130);
				}
				if (player->GetHealth() <= 0)
				{
					DisplayText(renderer, font, "YOU LOSE!!", 255, 0, 0, 50, 100);
					DisplayText(renderer, font, "Press BACKSPACE to go back to the main menu", 255, 0, 0, 50, 130);
				}
				// PRESENT TO SCREEN //
				SDL_RenderPresent(renderer);

				if (player->m_RequestQuit) // free memory
				{
					gamerunning = false;
					Mix_FreeMusic(map->m_Music);
					Mix_FreeChunk(map->m_ShotSound);
					Mix_FreeChunk(map->m_HurtSound);
					delete map;
					delete player;
				}
			}
			// // END OF GAMEPLAY LOOP // //
		}
		
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	SDL_Quit();
	return 0;
}


void DisplayText(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, int _r, int _g, int _b, int _x, int _y)
{
	SDL_Surface* surf = TTF_RenderText_Solid(_font, _text, SDL_Color{ 0,255,0 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surf);
	SDL_Rect textbox = { _x, _y, surf->w, surf->h };
	SDL_RenderCopy(_renderer, texture, NULL, &textbox);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);
}