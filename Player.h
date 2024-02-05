#pragma once
#include <SDL_ttf.h>
#include "CharacterObject.h"

class Player : public CharacterObject
{
public:
	Player(int _health, Vector2D _pos, MapObj* _map, SDL_Renderer* _renderer, const char* _spriteFile);
	~Player();
	Vector2D movementDesired;
	float m_Cooldown = 0;
	int m_mousex, m_mousey;
	bool m_Shooting = false;
	int m_score = 0;
	SDL_Event m_Event;
	bool m_RequestQuit = false;

	void TakeDamage(int _damage);
	void DoTick();
	void Shoot();
	void Movement();
};

