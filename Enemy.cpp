#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(int _health, Vector2D _pos, MapObj* _map, SDL_Renderer* _renderer, Player* _player)
{
	SetHealth(_health);
	SetPosition(_pos);
	m_MapObj = _map;
	m_Renderer = _renderer;
	m_Player = _player;
	m_Sprite = new Sprite(m_Renderer, m_SpriteFile, GetPosition().x, GetPosition().y, 30, 30);
}

Enemy::~Enemy()
{
	if (m_Sprite) delete m_Sprite;
	m_Player->m_score += 10;
}

void Enemy::DoTick()
{
	CharacterObject::DoTick();
	if (m_Player->GetHealth() > 0) Shoot();
}

void Enemy::Shoot()
{
	m_Cooldown -= 1 * m_MapObj->getDelta();
	if (m_Cooldown <= 0)
	{
		if (!m_MapObj->RaycastCheck(GetPosition(), m_Player->GetPosition()))
		{
			m_MapObj->m_BulletList.push_back(new Bullet(false, 20, 300, (m_Player->GetPosition() - GetPosition()).GetUnit(), GetPosition(), m_MapObj, m_Renderer));
			Mix_PlayChannel(-1, m_MapObj->m_ShotSound, 0);
		}
		m_Cooldown = 0.5f;
	}
}
