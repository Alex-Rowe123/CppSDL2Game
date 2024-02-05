#pragma once
#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Sprite();

	void UpdateLocation(int _x, int _y);
	void DrawToScreen();

private:
	SDL_Renderer* m_Renderer;
	SDL_Texture* m_Texture;
	SDL_Rect m_ImgRect;

};

