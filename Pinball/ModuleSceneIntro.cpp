#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
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

	App->physics->Enable();
	App->player->Enable();

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/Background.png");
	font = App->fonts->Load("pinball/nesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);
	bumperFx = App->audio->LoadFx("pinball/audio/fx/Bumper.wav");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//BACKGROUND -------------------------------------------------------------------------------------
	backgrounds.add(App->physics->CreateChain(0, 0, backgroundChain, 76, b2_staticBody));
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

	Bumper* b = new Bumper;
	b->bumpy = App->physics->CreateCircle(152, 198, 20, b2_staticBody);
	b->bumpy->body->GetFixtureList()->SetRestitution(1.1f);
	b->bumpy->listener = this;
	b->animation.PushBack({ 12,76,44,48 });
	b->animation.PushBack({ 60,76,44,48 });
	bumpers.add(b);

	Bumper* b2 = new Bumper;
	b2->bumpy = App->physics->CreateCircle(104, 640, 20, b2_staticBody);
	b2->bumpy->body->GetFixtureList()->SetRestitution(1.1f);
	b2->bumpy->listener = this;
	b2->animation.PushBack({ 12,76,44,48 });
	b2->animation.PushBack({ 60,76,44,48 });
	bumpers.add(b2);

	Bumper* b3 = new Bumper;
	b3->bumpy = App->physics->CreateCircle(152, 704, 20, b2_staticBody);
	b3->bumpy->body->GetFixtureList()->SetRestitution(1.1f);
	b3->bumpy->listener = this;
	b3->animation.PushBack({ 110,78,44,48 });
	b3->animation.PushBack({ 158,78,44,48 });
	bumpers.add(b3);

	Bumper* b4 = new Bumper;
	b4->bumpy = App->physics->CreateCircle(200, 640, 20, b2_staticBody);
	b4->bumpy->body->GetFixtureList()->SetRestitution(1.1f);
	b4->bumpy->listener = this;
	b4->animation.PushBack({ 12,76,44,48 });
	b4->animation.PushBack({ 60,76,44,48 });
	bumpers.add(b4);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::PreUpdate() {
	p2List_item<Bumper*>* bumperAnimations = bumpers.getFirst();
	while (bumperAnimations != NULL)
	{
		bumperAnimations->data->animation.Reset();
		bumperAnimations = bumperAnimations->next;
	}
	return UPDATE_CONTINUE;
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

	int x, y;
	App->player->kicker.mobile->GetPosition(x, y);
	App->renderer->Blit(background, x, y - 88, false, &App->player->kickerSect);

	p2List_item<Bumper*>* b = bumpers.getFirst();
	while (b != NULL)
	{
		int x, y;
		b->data->bumpy->GetPosition(x, y);
		App->renderer->Blit(App->player->playerText, x - 2, y - 4, false, &b->data->animation.GetCurrentFrame() , false, 1.0f, b->data->bumpy->GetRotation());
		b = b->next;
	}

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
	p2List_item<Bumper*>* b = bumpers.getFirst();
	while (b != NULL)
	{
		if (bodyA == b->data->bumpy && bodyB->listener == (Module*)App->player)
		{
			App->audio->PlayFx(bumperFx);
			//// 1
			//b2Vec2 body;
			//body.x = METERS_TO_PIXELS(bodyA->body->GetPosition().x - bodyB->body->GetPosition().x);
			//body.y = METERS_TO_PIXELS(bodyA->body->GetPosition().y - bodyB->body->GetPosition().y);
			//// 2
			//b2Vec2 forceDirection = bodyB->body->GetWorldVector(b2Vec2(0, 1));
			//forceDirection = 1.0f * forceDirection;
			//bodyB->body->ApplyForce(forceDirection,bodyB->body->GetPosition(), true);
			b->data->animation.Update();
		}
		b = b->next;
	}
}