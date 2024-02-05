#pragma once
#include <iostream>
#include "Map.h"
#include "Vector2D.h"
#include "Sprite.h"

class Bullet;

class CharacterObject
{
public:
	CharacterObject();
	void TakeDamage(int _damage);
	void ApplyForce(Vector2D _force);
	void SetForce(Vector2D _force);
	void SetHealth(int _health);
	void SetPosition(Vector2D _pos);
	int GetHealth();
	Vector2D GetPosition();
	Vector2D GetVelocity();
	void DoTick();
	MapObj* m_MapObj;
	SDL_Renderer* m_Renderer;
	bool m_RequestDelete = false;

protected:
	Vector2D m_Velocity = Vector2D(0, 0);
	Sprite* m_Sprite;
	float m_MaxVelocity = 1000.0f;
	float m_Decceleration = 0.7f;
	float m_SpeedMultiplier = 200.0f;
	Vector2D m_Pos;

private:
	int m_Health;
	void SolveVelocity();
};



class Bullet : public CharacterObject
{
public:
	Bullet(bool _friendly, int _damage, float _speed, Vector2D _dir, Vector2D _start, MapObj* _map, SDL_Renderer* _renderer);
	~Bullet();
	void DoTick();
	const char* m_SpriteFile = "bullet.bmp";

private:
	bool m_Friendly;
	int m_Damage;
	Vector2D m_Direction;
	Vector2D m_Start;
	void SolveVelocity();
	void CheckForEnemy();
};