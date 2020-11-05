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
	click_fx = App->audio->LoadFx("pinball/audio/fx/click.wav");

	p2List_item<PhysBody*>* bodyB;
	p2List_item<PhysBody*>* bodyA;
	b2Vec2 a = { -0.44, 0 };
	b2Vec2 b = { 0, 0 };

	bodyB = handlesCircles.add(App->physics->CreateCircle(82, 868, 10, b2_staticBody));
	bodyA = handlesRects.add(App->physics->CreateRectangle(72 + rectSect.w/2, 858 + rectSect.h/2, rectSect.w, rectSect.h, b2_dynamicBody));
	App->physics->CreateRevoluteJoint(bodyA->data, a, bodyB->data, b, 35.0f);

	bodyB = handlesCircles.add(App->physics->CreateCircle(82, 410, 10, b2_staticBody));
	bodyA = handlesRects.add(App->physics->CreateRectangle(72 + rectSect.w / 2, 400 + rectSect.h / 2, rectSect.w, rectSect.h, b2_dynamicBody));
	App->physics->CreateRevoluteJoint(bodyA->data, a, bodyB->data, b, 35.0f);

	a = { 0.44,0 };

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

	// Blits
	p2List_item<PhysBody*>* c = circles.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(playerText, x, y, false, &circleSect, false, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	p2List_item<PhysBody*>* h = handlesRects.getFirst();
	while (h != NULL)
	{
		int x, y;
		h->data->GetPosition(x, y);
		App->renderer->Blit(playerText, x, y, false, &rectSect, false, 1.0f, h->data->GetRotation());
		h = h->next;
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(click_fx);
}