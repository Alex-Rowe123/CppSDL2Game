#include "Map.h"
#include "Enemy.h"
#include <SDL.h>

MapObj::MapObj(SDL_Renderer* _renderer)
{
	m_CurrentTime = 0;
	m_LastTime = SDL_GetTicks64();
	m_DeltaTime = 0;
	m_renderer = _renderer;

	m_ShotSound = Mix_LoadWAV("shot.wav");
	m_HurtSound = Mix_LoadWAV("hurt.wav");
	m_Music = Mix_LoadMUS("music.wav");
	Mix_PlayMusic(m_Music, -1);
}

MapObj::~MapObj()
{
	for (std::vector<Enemy*>::iterator i = m_EnemyList.begin(); i != m_EnemyList.end(); i++) // loop through enemylist vector
	{
		delete (*i); // delete bullet object
	}
	m_EnemyList.clear(); // shrink array to new size
	for (std::vector<Bullet*>::iterator i = m_BulletList.begin(); i != m_BulletList.end(); i++) // loop through enemylist vector
	{
		delete (*i); // delete bullet object
	}
	m_BulletList.clear(); // shrink array to new size
}

void MapObj::EnemyDoTick()
{
	for (auto& const i : m_EnemyList) i->DoTick(); // do tick for all enemies

	for (std::vector<Enemy*>::iterator i = m_EnemyList.begin(); i != m_EnemyList.end();) // loop through enemylist vector
	{
		if ((*i)->m_RequestDelete == true) // if the object is requested for deletion
		{
			delete (*i); // delete bullet object
			i = m_EnemyList.erase(i); // remove bullet requested for deletion from bullet vector and make iterator next valid index
		}
		else ++i;
	}
	m_EnemyList.shrink_to_fit(); // shrink array to new size
}

void MapObj::BulletDoTick()
{
	for (auto &const i : m_BulletList) i->DoTick(); // do tick for all bullets
	for (std::vector<Bullet*>::iterator i = m_BulletList.begin(); i != m_BulletList.end();) // loop through bulletlist vector
	{
		if ((*i)->m_RequestDelete == true) // if the object is requested for deletion
		{
			delete (*i); // delete bullet object
			i = m_BulletList.erase(i); // remove bullet requested for deletion from bullet vector and make iterator next valid index
		}
		else ++i;
	}
	m_BulletList.shrink_to_fit(); // shrink array to new size
}

void MapObj::calculateDelta()
{
	m_LastTime = m_CurrentTime;
	m_CurrentTime = SDL_GetTicks64();
	m_DeltaTime = (m_CurrentTime - m_LastTime) / 1000; // delta time is the time between the current and last time divided by 1000 to become decimal
}

double MapObj::getDelta()
{
	return m_DeltaTime;
}

Vector2D MapObj::Raycast(Vector2D _start, Vector2D _end)
{
	Vector2D dir = _end - _start;
	for (float i{ 0 }; i < 1; i += 0.01)
	{
		if (m_MapGrid[int((_start + dir * i).y / 45)][int((_start + dir * i).x / 45)] == 1) // check if position is inside wall
		{
			return dir * i;
		}
	}
	return _end;
}

bool MapObj::RaycastCheck(Vector2D _start, Vector2D _end)
{
	Vector2D dir = _end - _start;
	for (float i{ 0 }; i < 1; i += 0.01)
	{
		if (m_MapGrid[int((_start + dir * i).y / 45)][int((_start + dir * i).x / 45)] == 1) // check if position is inside wall
		{
			return true;
		}
	}
	return false;
}

void MapObj::GenerateMap()
{
	for (int i{ 0 }; i < 20; ++i)
	{
		for (int j{ 0 }; j < 20; ++j)
		{
			if (m_MapGrid[j][i] == false && !(rand() % 11)) // if empty map space and 1/11 chance
			{
				SpawnEnemy(Vector2D((i+1) * 45 - 25, (j+1) * 45 - 25)); // spawn at center of grid square
			}
		}
	}
}

void MapObj::SpawnEnemy(Vector2D _pos)
{
	m_EnemyList.push_back(new Enemy(100, _pos, this, m_renderer, m_Player));
}
