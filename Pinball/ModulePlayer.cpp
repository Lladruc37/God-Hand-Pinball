#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	playerText = App->textures->Load("pinball/GameElements.png");
	click_fx = App->audio->LoadFx("pinball/audio/fx/BallCollision.wav");

	b2Vec2 a = { -0.44, 0 };
	b2Vec2 b = { 0, 0 };

	// HANDLES --------------------------------------------------------------
	Handle* h = new Handle;
	h->Circle = App->physics->CreateCircle(82, 868, 4, b2_staticBody);
	h->Rect = App->physics->CreateRectangle(72 + rectSect.w / 2, 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	h->rightSide = false;
	App->physics->CreateRevoluteJoint(h->Rect, a, h->Circle, b, 35.0f);
	handles.add(h);

	Handle* h2 = new Handle;
	h2->Circle = App->physics->CreateCircle(82, 410, 4, b2_staticBody);
	h2->Rect = App->physics->CreateRectangle(72 + rectSect.w / 2, 400 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	h2->rightSide = false;
	App->physics->CreateRevoluteJoint(h2->Rect, a, h2->Circle, b, 35.0f);
	handles.add(h2);

	a = { 0.44,0 };

	Handle* h3 = new Handle;
	h3->Circle = App->physics->CreateCircle(226, 868, 4, b2_staticBody);
	h3->Rect = App->physics->CreateRectangle(216 - rectSect.w / 2, 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	h3->rightSide = true;
	App->physics->CreateRevoluteJoint(h3->Rect, a, h3->Circle, b, 35.0f);
	handles.add(h3);

	Handle* h4 = new Handle;
	h4->Circle = App->physics->CreateCircle(223, 410, 4, b2_staticBody);
	h4->Rect = App->physics->CreateRectangle(213 - rectSect.w / 2, 400 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	h4->rightSide = true;
	App->physics->CreateRevoluteJoint(h4->Rect, a, h4->Circle, b, 35.0f);
	handles.add(h4);

	//KICKER -----------------------------------------------------------------------------------------
	kicker.pivot = App->physics->CreateRectangle(313, 894, 20, 8, b2_staticBody);
	kicker.mobile = App->physics->CreateRectangle(304, 794, 22, 8, b2_dynamicBody);
	App->physics->CreatePrismaticJoint(kicker.mobile, { 0,0 }, kicker.pivot, { 0,0 }, { 0,1 }, 1.9f);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		circles.getLast()->data->listener = this;
	}

	// HANDLE CONTROL
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		p2List_item<Handle*>* h = handles.getFirst();
		while (h != NULL)
		{
			if (h->data->rightSide == false)
			{
				h->data->Rect->body->ApplyForce({-3,0}, {0,0}, true);
			}
			h = h->next;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		p2List_item<Handle*>* h = handles.getFirst();
		while (h != NULL)
		{
			if (h->data->rightSide == true)
			{
				h->data->Rect->body->ApplyForce({ 3,0 }, { 0,0 }, true);
			}
			h = h->next;
		}
	}

	//KICKER CONTROL
	kicker.mobile->body->ApplyForce({ 0,-18 }, { 0,0 }, true);
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		kicker.mobile->body->ApplyForce({ 0,18 }, { 0,0 }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		kicker.mobile->body->ApplyForce({ 0,-150 }, { 0,0 }, true);
	}

	// Blits
	p2List_item<PhysBody*>* c = circles.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(playerText, x, y, false, &circleSect, false, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	p2List_item<Handle*>* h = handles.getFirst();
	while (h != NULL)
	{
		int x, y;
		h->data->Rect->GetPosition(x, y);
		App->renderer->Blit(playerText, x, y - 5, false, &rectSect, h->data->rightSide, 1.0f, h->data->Rect->GetRotation());
		h = h->next;
	}



	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(click_fx);
}