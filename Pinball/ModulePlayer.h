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
	SDL_Texture* playerText;

	p2List<PhysBody*> circles;
	SDL_Rect circleSect = { 127, 317, 19, 21 };

	p2List<PhysBody*> handlesRects;
	p2List<PhysBody*> handlesCircles;
	SDL_Rect rectSect = { 48, 250, 64, 20 };

	uint click_fx;
};