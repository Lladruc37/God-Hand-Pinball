#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleTransition.h"
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
	fallFx = App->audio->LoadFx("pinball/audio/fx/PeachFall.wav");

	deathSensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 10, b2_staticBody);
	deathSensor->listener = this;

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

	bumperTimer = 0;
	Bumper* b = new Bumper;
	b->bumpy = App->physics->CreateCircle(152, 198, 20, b2_staticBody);
	b->bumpy->listener = this;
	b->animation.PushBack({ 12,76,44,48 });
	b->animation.PushBack({ 60,74,44,48 });
	bumpers.add(b);

	Bumper* b2 = new Bumper;
	b2->bumpy = App->physics->CreateCircle(104, 640, 20, b2_staticBody);
	b2->bumpy->listener = this;
	b2->animation.PushBack({ 12,76,44,48 });
	b2->animation.PushBack({ 60,74,44,48 });
	bumpers.add(b2);

	Bumper* b3 = new Bumper;
	b3->bumpy = App->physics->CreateCircle(152, 704, 20, b2_staticBody);
	b3->bumpy->listener = this;
	b3->animation.PushBack({ 110,78,44,48 });
	b3->animation.PushBack({ 158,74,44,48 });
	bumpers.add(b3);

	Bumper* b4 = new Bumper;
	b4->bumpy = App->physics->CreateCircle(200, 640, 20, b2_staticBody);
	b4->bumpy->listener = this;
	b4->animation.PushBack({ 12,76,44,48 });
	b4->animation.PushBack({ 60,76,44,48 });
	bumpers.add(b4);

	App->audio->PlayMusic("pinball/audio/music/silence.ogg", 1.0f);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->player->Disable();
	App->physics->Disable();

	App->textures->Unload(background);
	App->fonts->Unload(font);

	return true;
}

update_status ModuleSceneIntro::PreUpdate() {
	p2List_item<Bumper*>* bumperAnimations = bumpers.getFirst();
	while (bumperAnimations != NULL)
	{
		if (bumperAnimations->data->animation.currentFrame == 1)
		{
			if (bumperTimer == 5)
			{
				bumperTimer = 0;
				bumperAnimations->data->animation.Reset();
			}
			else
			{
				bumperTimer++;
			}
		}
		bumperAnimations = bumperAnimations->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->transition->Transition(this, (Module*)App->title_screen);
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

	// Text UI ----------------
	sprintf_s(prevScore, 8, "P-SCORE");
	App->fonts->BlitText(fontSize * 2, fontSize * 1, font, prevScore);
	sprintf_s(prevScoreNum, 12, "%6d", App->player->previousScore);
	App->fonts->BlitText(fontSize * 2.5, fontSize * 2, font, prevScoreNum);

	sprintf_s(currentScore, 6, "SCORE");
	App->fonts->BlitText(fontSize * 11, fontSize * 1, font, currentScore);
	sprintf_s(currentScoreNum, 12, "%6d", App->player->currentScore);
	App->fonts->BlitText(fontSize * 10.5, fontSize * 2, font, currentScoreNum);

	sprintf_s(highScore, 8, "H-SCORE");
	App->fonts->BlitText(fontSize * 18, fontSize * 1, font, highScore);
	sprintf_s(highScoreNum, 12, "%6d", App->player->highScore);
	App->fonts->BlitText(fontSize * 18.5, fontSize * 2, font, highScoreNum);

	sprintf_s(balls, 6, "BALLS");
	App->fonts->BlitText(fontSize * 19.5, fontSize * 77, font, balls);
	sprintf_s(ballsNum, 2, "%1d", App->player->ballCount);
	App->fonts->BlitText(fontSize * 21.5, fontSize * 78, font, ballsNum);

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

	/*if (App->player->ballCount < 0)
	{
		transition to game over screen
	}*/
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
			b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
			force *= 3;
			bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);
			b->data->animation.Update();
			App->player->currentScore += 100;
			return;
		}
		b = b->next;
	}

	if (bodyA == deathSensor && bodyB->listener == (Module*)App->player)
	{
		App->player->ballCount--;
		App->player->circles.add(App->physics->CreateCircle(303, 765, 10));
		App->player->circles.getLast()->data->listener = (Module*)App->player;
		App->audio->PlayFx(fallFx);
		return;
	}
}