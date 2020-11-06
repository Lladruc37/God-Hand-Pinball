#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "p2Point.h"

struct Handle
{
	PhysBody* Rect;
	PhysBody* Circle;
	bool rightSide;
};

struct Kicker
{
	PhysBody* pivot;
	PhysBody* mobile;
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = false);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	SDL_Texture* playerText;

	p2List<PhysBody*> circles;
	SDL_Rect circleSect = { 127, 317, 19, 21 };

	p2List<Handle*> handles;
	SDL_Rect rectSect = { 48, 250, 64, 20 };

	Kicker kicker;
	SDL_Rect kickerSect = { 302,706,22,96 };

	uint click_fx;
};