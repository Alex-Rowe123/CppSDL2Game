#include "Player.h"

Player::Player(int _health, Vector2D _pos, MapObj* _map, SDL_Renderer* _renderer, const char* _spriteFile)
{
	m_mousex = 0;
	m_mousey = 0;
	SetHealth(_health);
	SetPosition(_pos);
	m_MapObj = _map;
	m_Renderer = _renderer;
	m_Sprite = new Sprite(m_Renderer, _spriteFile, GetPosition().x, GetPosition().y, 30, 30);// create sprite object
}

Player::~Player()
{
	if (m_Sprite) delete m_Sprite;
}

void Player::Movement()
{
	// apply force in desired direction multiplied by the speed multiplier
	ApplyForce(movementDesired * m_SpeedMultiplier);
}

void Player::Shoot()
{
	if (m_Cooldown <= 0) // if cooldown is over
	{
		// spawn bullet
		m_MapObj->m_BulletList.push_back(new Bullet(true, 50, 300, (Vector2D(m_mousex, m_mousey) - GetPosition()).GetUnit(), GetPosition(), m_MapObj, m_Renderer));
		Mix_PlayChannel(-1,m_MapObj->m_ShotSound,0);
		m_Cooldown = 0.2; // reset cooldown
	}
}

void Player::DoTick()
{
	SDL_PollEvent(&m_Event);
	// KEYBOARD HANDLING
	auto keys = SDL_GetKeyboardState(NULL); // Get state of keyboard
	if (GetHealth() > 0)
	{
		m_Cooldown -= 1.0f * m_MapObj->getDelta(); // cooldown for shooting
		movementDesired = Vector2D(0.0f, 0.0f); // Reset desired movement
		if (keys[SDL_SCANCODE_W]) movementDesired.y = -1;
		if (keys[SDL_SCANCODE_S]) movementDesired.y = 1;
		if (keys[SDL_SCANCODE_D]) movementDesired.x = 1;
		if (keys[SDL_SCANCODE_A]) movementDesired.x = -1;

		// If the magnitude of the desired movement exceeds ~1.0f, clamp it to a magnitude of ~1.0f;
		if (movementDesired.GetMagnitude() > 1.01f) movementDesired = movementDesired.GetUnit();

		// MOUSE HANDLING //
		SDL_GetMouseState(&m_mousex, &m_mousey);

		if (m_Event.type == SDL_MOUSEBUTTONDOWN) m_Shooting = true;
		else if (m_Event.type == SDL_MOUSEBUTTONUP) m_Shooting = false;
		if (m_Shooting) Shoot();
		CharacterObject::DoTick();
	}
	if (keys[SDL_SCANCODE_BACKSPACE]) m_RequestQuit = true;
}

void Player::TakeDamage(int _damage)
{
	CharacterObject::TakeDamage(_damage);
	m_score -= 5;
}
