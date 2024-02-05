#pragma once
#include "CharacterObject.h"


class Enemy : public CharacterObject
{
public:
	Enemy(int _health, Vector2D _pos, MapObj* _map, SDL_Renderer* _renderer, Player* _player);
	~Enemy();
	void DoTick();
	const char* m_SpriteFile = "enemytest.bmp";
	
private:
	void Shoot();
	float m_Cooldown{0};
	Player* m_Player;
};

