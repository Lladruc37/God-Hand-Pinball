#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
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

	playerText = App->textures->Load("Game/pinball/GameElements.png");
	clickFx = App->audio->LoadFx("Game/pinball/audio/fx/BallCollision.wav");
	kickerFx = App->audio->LoadFx("Game/pinball/audio/fx/Spring.wav");

	b2Vec2 a = { -0.44, 0 };
	b2Vec2 b = { 0, 0 };

	// Flippers --------------------------------------------------------------
	Flipper* f = new Flipper;
	f->Circle = App->physics->CreateCircle(82, 868, 4, b2_staticBody);
	f->Rect = App->physics->CreateRectangle(72 + rectSect.w / 2, 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	f->rightSide = false;
	App->physics->CreateRevoluteJoint(f->Rect, a, f->Circle, b, 35.0f);
	flippers.add(f);

	Flipper* f2 = new Flipper;
	f2->Circle = App->physics->CreateCircle(82, 410, 4, b2_staticBody);
	f2->Rect = App->physics->CreateRectangle(72 + rectSect.w / 2, 400 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	f2->rightSide = false;
	App->physics->CreateRevoluteJoint(f2->Rect, a, f2->Circle, b, 35.0f);
	flippers.add(f2);

	a = { 0.44,0 };

	Flipper* f3 = new Flipper;
	f3->Circle = App->physics->CreateCircle(226, 868, 4, b2_staticBody);
	f3->Rect = App->physics->CreateRectangle(216 - rectSect.w / 2, 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	f3->rightSide = true;
	App->physics->CreateRevoluteJoint(f3->Rect, a, f3->Circle, b, 35.0f);
	flippers.add(f3);

	Flipper* f4 = new Flipper;
	f4->Circle = App->physics->CreateCircle(223, 410, 4, b2_staticBody);
	f4->Rect = App->physics->CreateRectangle(213 - rectSect.w / 2, 400 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	f4->rightSide = true;
	App->physics->CreateRevoluteJoint(f4->Rect, a, f4->Circle, b, 35.0f);
	flippers.add(f4);

	//Kicker -----------------------------------------------------------------------------------------
	kicker.pivot = App->physics->CreateRectangle(313, 894, 20, 8, b2_staticBody);
	kicker.mobile = App->physics->CreateRectangle(304, 794, 22, 8, b2_dynamicBody);
	App->physics->CreatePrismaticJoint(kicker.mobile, { 0,0 }, kicker.pivot, { 0,0 }, { 0,1 }, 1.9f);

	//Ball -------------------------------------------------------------------------------------------
	onceInit = true;
	isDead = false;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(playerText);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->scene_intro->currentScene == Scene::PINBALL)
	{
		if (onceInit)
		{
			circles.add(App->physics->CreateCircle(303, 767, 10));
			circles.getLast()->data->listener = this;
			currentScore = 0;
			onceInit = false;
			isDead = false;
			onceBall = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->physics->debug)
		{
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
			circles.getLast()->data->listener = this;
		}

		// Flippers --------------------------------------------------
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			p2List_item<Flipper*>* f = flippers.getFirst();
			while (f != NULL)
			{
				if (f->data->rightSide == false)
				{
					f->data->Rect->body->ApplyForce({ -3,0 }, { 0,0 }, true);
				}
				f = f->next;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			p2List_item<Flipper*>* f = flippers.getFirst();
			while (f != NULL)
			{
				if (f->data->rightSide == true)
				{
					f->data->Rect->body->ApplyForce({ 3,0 }, { 0,0 }, true);
				}
				f = f->next;
			}
		}

		// Kicker --------------------------------------------------------
		kicker.mobile->body->ApplyForce({ 0,-18 }, { 0,0 }, true);
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			kicker.mobile->body->ApplyForce({ 0,18 }, { 0,0 }, true);
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			kicker.mobile->body->ApplyForce({ 0,-110 }, { 0,0 }, true);
			App->audio->PlayFx(kickerFx);
		}

		// Score related ----------------------------------------------------
		if (currentScore > 999990)
		{
			currentScore = 999990;
		}
		if (currentScore > highScore)
		{
			highScore = currentScore;
		}
		
		if (currentScore != 0 && currentScore > 50000)
		{
			if (onceBall)
			{
				ballCount++;
				onceBall = false;
			}
		}

		// Game Overs ----------------------------------------------------
		if (isDead)
		{
			isDead = false;
			if (App->physics->mouse_joint != nullptr)
			{
				App->physics->mouse_joint->GetBodyA()->GetWorld()->DestroyJoint(App->physics->mouse_joint);
				App->physics->mouse_joint = nullptr;
			}
			p2List_item<PhysBody*>* c = circles.getFirst();
			while (c != NULL)
			{
				c->data->body->GetWorld()->DestroyBody(c->data->body);
				c = c->next;
			}
			circles.clear();

			ballCount--;
			if (ballCount >= 0)
			{
				circles.add(App->physics->CreateCircle(303, 765, 10));
				circles.getLast()->data->listener = (Module*)App->player;
			}
		}

		// Blits ---------------------------------------------------------
		p2List_item<PhysBody*>* c = circles.getFirst();
		while (c != NULL)
		{
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(playerText, x, y, false, &circleSect, false, 1.0f, c->data->GetRotation());
			c = c->next;
		}

		if (currentScore < 100000 || currentScore >= 150000)
		{
			p2List_item<Flipper*>* f = flippers.getFirst();
			while (f != NULL)
			{
				int x, y;
				f->data->Rect->GetPosition(x, y);

				App->renderer->Blit(playerText, x, y - 5, false, &rectSect, f->data->rightSide, 1.0f, f->data->Rect->GetRotation());

				f = f->next;
			}
		}
	}
	else
	{
		previousScore = currentScore;
		onceInit = true;
		isDead = false;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(clickFx);
}