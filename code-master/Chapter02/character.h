#pragma once
#include"Actor.h"

class character : public Actor
{
public:
	character(class Game* game);
	void UpdateActor(float deltaTime) override;

};