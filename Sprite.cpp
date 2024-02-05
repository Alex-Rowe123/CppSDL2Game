#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h)
{
	m_Renderer = _renderer;
	SDL_Surface* bmp = SDL_LoadBMP(_file);
	m_Texture = SDL_CreateTextureFromSurface(m_Renderer, bmp);
	SDL_FreeSurface(bmp);
	m_ImgRect.x = _y;
	m_ImgRect.y = _x;
	m_ImgRect.w = _w;
	m_ImgRect.h = _h;
}

Sprite::~Sprite()
{
	if (m_Texture) SDL_DestroyTexture(m_Texture);
}

void Sprite::UpdateLocation(int _x, int _y)
{
	if (&m_ImgRect != NULL) // make sure it exists
	{
		// minus offset so that the center of the sprite is at the location of the object
		m_ImgRect.x = _x - (m_ImgRect.h / 2);
		m_ImgRect.y = _y - (m_ImgRect.w / 2);
	}
}

void Sprite::DrawToScreen()
{
	// if texture is valid draw to screen
	if (m_Texture) SDL_RenderCopy(m_Renderer, m_Texture, NULL, &m_ImgRect);
}
