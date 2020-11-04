#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/Background.png");
	font = App->fonts->Load("pinball/nesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	backgrounds.add(App->physics->CreateChain(0, 0, backgroundChain, 90, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftBlue, 30, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopRightSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopRightBig, 72, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopRightGreen, 22, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, MidLeft, 20, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, FirstCardPellet, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, SecondCardPellet, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, ThirdCardPellet, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, FourthCardPellet, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, FifthCardPellet, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, SixthCardPellet, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, BottomLeftPink, 12, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, BottomRightPink, 12, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, BottomLeftWhite, 16, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, BottomRightWhite, 16, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// -------------------------------------------------------------------------
	// All draw functions ------------------------------------------------------
	// -------------------------------------------------------------------------
	SDL_Rect sect = { 350, 0, 336, 954 };
	App->renderer->Blit(background, 0, 0, true, &sect);

	sprintf_s(test, 16, "You're a qt uwu");
	App->fonts->BlitText(32, 32, font, test);


	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//This will be a fun function in the future :)
}