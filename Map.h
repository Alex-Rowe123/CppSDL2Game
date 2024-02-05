#pragma once
#include <vector>
#include <iterator>
#include <ctime>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Vector2D.h"

class Player;
class Enemy;
class Bullet;

class MapObj
{
public:
	MapObj(SDL_Renderer* _renderer);
	~MapObj();
	bool m_MapGrid[30][30]{0};
	Player* m_Player{NULL};
	std::vector<Enemy*> m_EnemyList;
	std::vector<Bullet*> m_BulletList;
	void EnemyDoTick();
	void BulletDoTick();
	void calculateDelta();
	double getDelta();
	Vector2D Raycast(Vector2D _start, Vector2D _end);
	bool RaycastCheck(Vector2D _start, Vector2D _end);
	void GenerateMap();
	Mix_Chunk* m_ShotSound;
	Mix_Chunk* m_HurtSound;
	Mix_Music* m_Music;

private:
	void SpawnEnemy(Vector2D _pos);
	double m_CurrentTime;
	double m_LastTime;
	double m_DeltaTime;
	SDL_Renderer* m_renderer;
};