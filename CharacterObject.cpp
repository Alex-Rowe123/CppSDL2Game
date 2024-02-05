#include "CharacterObject.h"
#include "Enemy.h"
#include "Player.h"

CharacterObject::CharacterObject()
{
	SetPosition(Vector2D(0, 0));
};

void CharacterObject::TakeDamage(int _damage)
{
	m_Health -= _damage;
	Mix_PlayChannel(-1, m_MapObj->m_HurtSound, 0);
	if (m_Health <= 0) m_RequestDelete = true;
}

void CharacterObject::ApplyForce(Vector2D _force)
{
	m_Velocity = m_Velocity + _force;
}

void CharacterObject::SetForce(Vector2D _force)
{
	m_Velocity = _force;
}

void CharacterObject::SetHealth(int _health)
{
	m_Health = _health;
}

void CharacterObject::SetPosition(Vector2D _pos)
{
	m_Pos = _pos;
}

int CharacterObject::GetHealth()
{
	return m_Health;
}

Vector2D CharacterObject::GetPosition()
{
	return m_Pos;
}

Vector2D CharacterObject::GetVelocity()
{
	return m_Velocity;
}

void CharacterObject::DoTick()
{
	SolveVelocity();
	m_Sprite->UpdateLocation(m_Pos.x, m_Pos.y);
	m_Sprite->DrawToScreen();
}

void CharacterObject::SolveVelocity()
{
	m_Velocity = m_Velocity * (m_Decceleration * m_MapObj->getDelta()); // multiply the vector by the decceleration factor

	/* Collision checks are carried out by taking the actual position of the character, after accounting for velocity,
	and dividing it by the size of a grid square. That gives the square that the character will be on. It then checks the
	map grid to see if the square it is on contains a wall */

	if (m_MapObj->m_MapGrid[(int((m_Pos + m_Velocity).y))/45][(int((m_Pos + m_Velocity).x))/45] == 0) // check if character wont be inside wall grid square after movement
	{
		m_Pos = m_Pos + m_Velocity;
	}
	else // if it will be inside wall after movement, check to see if it can be moved a single axis
	{
		if (m_MapObj->m_MapGrid[(int((m_Pos).y)) / 45][(int((m_Pos + m_Velocity).x)) / 45] == 0) // check if character wont be inside wall grid square after movement in x direction
		{
			m_Pos = m_Pos + (m_Velocity * Vector2D(1.0f,0.0f)); // discard the y value
		}
		else if (m_MapObj->m_MapGrid[(int((m_Pos + m_Velocity).y)) / 45][(int((m_Pos).x)) / 45] == 0) // check if character wont be inside wall grid square after movement in y direction
		{
			m_Pos = m_Pos + (m_Velocity * Vector2D(0.0f, 1.0f)); // discard the x value
		}
		else m_Velocity = Vector2D(0.0f, 0.0f); // if it will collide, set the velocity to 0
	}
}

Bullet::Bullet(bool _friendly, int _damage, float _speed, Vector2D _dir, Vector2D _start, MapObj* _map, SDL_Renderer* _renderer)
{
	m_Friendly = _friendly;
	m_Damage = _damage;
	m_Direction = _dir;
	SetPosition(_start);
	ApplyForce(_dir * _speed);
	m_SpeedMultiplier = _speed;
	m_Decceleration = 1.0f;
	m_Start = _start;
	m_MapObj = _map;
	m_Renderer = _renderer;
	m_Sprite = new Sprite(m_Renderer, m_SpriteFile, GetPosition().x, GetPosition().y, 10, 10);
}

Bullet::~Bullet()
{
	if (m_Sprite) delete m_Sprite;
}

void Bullet::DoTick()
{
	if (!m_RequestDelete)
	{
		ApplyForce(m_Direction * m_SpeedMultiplier);
		if (m_Sprite)
		{
			m_Sprite->UpdateLocation(m_Pos.x, m_Pos.y);
			m_Sprite->DrawToScreen();
		}
		CheckForEnemy();
		SolveVelocity();
		if ((GetPosition() - m_Start).GetMagnitude() > 400) m_RequestDelete = true;
	}
}

void Bullet::SolveVelocity()
{
	m_Velocity = m_Velocity * (m_Decceleration * m_MapObj->getDelta()); // multiply the vector by the decceleration factor

	/* Collision checks are carried out by taking the actual position of the character, after accounting for velocity,
	and dividing it by the size of a grid square. That gives the square that the character will be on. It then checks the
	map grid to see if the square it is on contains a wall */

	if (m_MapObj->m_MapGrid[(int((m_Pos + m_Velocity).y)) / 45][(int((m_Pos + m_Velocity).x)) / 45] == 0) // check if character wont be inside wall grid square after movement
	{
		m_Pos = m_Pos + m_Velocity;
	}
	else m_RequestDelete = true;
}

void Bullet::CheckForEnemy()
{
	if (m_Friendly)
	{
		for (auto& const it : m_MapObj->m_EnemyList)
		{
			if ((it->GetPosition() - GetPosition()).GetMagnitude() < 20)
			{
				it->TakeDamage(m_Damage);
				m_RequestDelete = true;
			}
		}
	}
	else
	{
		if ((m_MapObj->m_Player->GetPosition() - GetPosition()).GetMagnitude() < 20)
		{
			m_MapObj->m_Player->TakeDamage(m_Damage);
			m_RequestDelete = true;
		}
	}
}
