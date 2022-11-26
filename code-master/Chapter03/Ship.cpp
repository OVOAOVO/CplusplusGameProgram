// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include"CircleComponent.h"
#include"Asteroid.h"
Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);
	mCollision = new CircleComponent(this);
	mCollision->SetRadius(35.0f);
}
float ff = 0.0f;
void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	for (auto ship_boom : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCollision, *(ship_boom->GetCircle())))
		{
			SetState(EDead);
			ship_boom->SetState(EDead); // –°–––«À¿Õˆ
			break;
		}
	}
	if (GetState()==EDead)
	{
		float a = 12.0f;
		if (a -= deltaTime < 0)
		{
			Ship* ship = new Ship(GetGame());
			ship->SetPosition(Vector2(300.0f, 400.0f));
			ship->SetRotation(0);

		}
	}
	ff = 0.0f;

}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
