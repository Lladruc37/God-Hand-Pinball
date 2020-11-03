#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	SDL_Texture* circleText;
	SDL_Rect circleSect = { 127,317,19,21 };
	uint bonus_fx;
};