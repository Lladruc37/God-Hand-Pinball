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
#include "time.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
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

	bumperFx = App->audio->LoadFx("pinball/audio/fx/Bumper.wav");
	fallFx = App->audio->LoadFx("pinball/audio/fx/PeachFall.wav");
	wallPacFx = App->audio->LoadFx("pinball/audio/fx/WallButton_PacMan.wav");
	buttonFx = App->audio->LoadFx("pinball/audio/fx/PointButton_SafetyButton.wav");
	sideBumperFx = App->audio->LoadFx("pinball/audio/fx/SideBumpers.wav");

	// Sensors
	Sensor* deathSensor = new Sensor;
	deathSensor->sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 10, b2_staticBody);
	deathSensor->sensor->listener = this;
	deathSensor->value = Sensor::DEATH;
	deathSensor->isActive = false;
	sensors.add(deathSensor);

	Sensor* pointSensor1 = new Sensor;
	pointSensor1->sensor = App->physics->CreateRectangleSensor(120, 120, 10, 10, b2_staticBody);
	pointSensor1->sensor->listener = this;
	pointSensor1->value = Sensor::HUNDREDS;
	pointSensor1->isActive = false;
	sensors.add(pointSensor1);

	Sensor* pointSensor2 = new Sensor;
	pointSensor2->sensor = App->physics->CreateRectangleSensor(152, 120, 10, 10, b2_staticBody);
	pointSensor2->sensor->listener = this;
	pointSensor2->value = Sensor::THOUSAND;
	pointSensor2->isActive = false;
	sensors.add(pointSensor2);

	Sensor* pointSensor3 = new Sensor;
	pointSensor3->sensor = App->physics->CreateRectangleSensor(184, 120, 10, 10, b2_staticBody);
	pointSensor3->sensor->listener = this;
	pointSensor3->value = Sensor::HUNDREDS;
	pointSensor3->isActive = false;
	sensors.add(pointSensor3);

	Sensor* cardSensor1 = new Sensor;
	cardSensor1->sensor = App->physics->CreateRectangleSensor(87, 580, 10, 10, b2_staticBody);
	cardSensor1->sensor->listener = this;
	cardSensor1->value = Sensor::CARD;
	cardSensor1->isActive = false;
	sensors.add(cardSensor1);

	Sensor* cardSensor2 = new Sensor;
	cardSensor2->sensor = App->physics->CreateRectangleSensor(120, 580, 10, 10, b2_staticBody);
	cardSensor2->sensor->listener = this;
	cardSensor2->value = Sensor::CARD;
	cardSensor2->isActive = false;
	sensors.add(cardSensor2);

	Sensor* cardSensor3 = new Sensor;
	cardSensor3->sensor = App->physics->CreateRectangleSensor(151, 580, 10, 10, b2_staticBody);
	cardSensor3->sensor->listener = this;
	cardSensor3->value = Sensor::CARD;
	cardSensor3->isActive = false;
	sensors.add(cardSensor3);

	Sensor* cardSensor4 = new Sensor;
	cardSensor4->sensor = App->physics->CreateRectangleSensor(183, 580, 10, 10, b2_staticBody);
	cardSensor4->sensor->listener = this;
	cardSensor4->value = Sensor::CARD;
	cardSensor4->isActive = false;
	sensors.add(cardSensor4);

	Sensor* cardSensor5 = new Sensor;
	cardSensor5->sensor = App->physics->CreateRectangleSensor(216, 580, 10, 10, b2_staticBody);
	cardSensor5->sensor->listener = this;
	cardSensor5->value = Sensor::CARD;
	cardSensor5->isActive = false;
	sensors.add(cardSensor5);

	Sensor* eggSensor1 = new Sensor;
	eggSensor1->sensor = App->physics->CreateRectangleSensor(111, 783, 10, 10, b2_staticBody);
	eggSensor1->sensor->listener = this;
	eggSensor1->value = Sensor::EGG;
	eggSensor1->isActive = false;
	sensors.add(eggSensor1);

	Sensor* eggSensor2 = new Sensor;
	eggSensor2->sensor = App->physics->CreateRectangleSensor(151, 783, 10, 10, b2_staticBody);
	eggSensor2->sensor->listener = this;
	eggSensor2->value = Sensor::EGG;
	eggSensor2->isActive = false;
	sensors.add(eggSensor2);

	Sensor* eggSensor3 = new Sensor;
	eggSensor3->sensor = App->physics->CreateRectangleSensor(191, 783, 10, 10, b2_staticBody);
	eggSensor3->sensor->listener = this;
	eggSensor3->value = Sensor::EGG;
	eggSensor3->isActive = false;
	sensors.add(eggSensor3);

	/*Sensor* holeSensor1 = new Sensor;
	holeSensor1->sensor = App->physics->CreateRectangleSensor(280, 360, 10, 10, b2_staticBody);
	holeSensor1->sensor->listener = this;
	holeSensor1->value = Sensor::TP;
	holeSensor1->isActive = false;
	sensors.add(holeSensor1);

	Sensor* holeSensor2 = new Sensor;
	holeSensor2->sensor = App->physics->CreateRectangleSensor(280, 535, 10, 10, b2_staticBody);
	holeSensor2->sensor->listener = this;
	holeSensor2->value = Sensor::TP;
	holeSensor2->isActive = false;
	sensors.add(holeSensor2);*/

	Sensor* pacSensor1 = new Sensor;
	pacSensor1->sensor = App->physics->CreateRectangleSensor(47, 87, 10, 10, b2_staticBody);
	pacSensor1->sensor->listener = this;
	pacSensor1->value = Sensor::PAC_MAN;
	pacSensor1->isActive = false;
	sensors.add(pacSensor1);

	Sensor* pacSensor2 = new Sensor;
	pacSensor2->sensor = App->physics->CreateRectangleSensor(36, 101, 10, 10, b2_staticBody);
	pacSensor2->sensor->listener = this;
	pacSensor2->value = Sensor::PAC_MAN;
	pacSensor2->isActive = false;
	sensors.add(pacSensor2);

	Sensor* pacSensor3 = new Sensor;
	pacSensor3->sensor = App->physics->CreateRectangleSensor(28, 117, 10, 10, b2_staticBody);
	pacSensor3->sensor->listener = this;
	pacSensor3->value = Sensor::PAC_MAN;
	pacSensor3->isActive = false;
	sensors.add(pacSensor3);

	Sensor* pacSensor4 = new Sensor;
	pacSensor4->sensor = App->physics->CreateRectangleSensor(24, 135, 10, 10, b2_staticBody);
	pacSensor4->sensor->listener = this;
	pacSensor4->value = Sensor::PAC_MAN;
	pacSensor4->isActive = false;
	sensors.add(pacSensor4);

	Sensor* pacSensor5 = new Sensor;
	pacSensor5->sensor = App->physics->CreateRectangleSensor(24, 153, 10, 10, b2_staticBody);
	pacSensor5->sensor->listener = this;
	pacSensor5->value = Sensor::PAC_MAN;
	pacSensor5->isActive = false;
	sensors.add(pacSensor5);

	Sensor* pacSensor6 = new Sensor;
	pacSensor6->sensor = App->physics->CreateRectangleSensor(26, 171, 10, 10, b2_staticBody);
	pacSensor6->sensor->listener = this;
	pacSensor6->value = Sensor::PAC_MAN;
	pacSensor6->isActive = false;
	sensors.add(pacSensor6);

	Sensor* pacSensor7 = new Sensor;
	pacSensor7->sensor = App->physics->CreateRectangleSensor(32, 187, 10, 10, b2_staticBody);
	pacSensor7->sensor->listener = this;
	pacSensor7->value = Sensor::PAC_MAN;
	pacSensor7->isActive = false;
	sensors.add(pacSensor7);

	Sensor* pacSensor8 = new Sensor;
	pacSensor8->sensor = App->physics->CreateRectangleSensor(38, 203, 10, 10, b2_staticBody);
	pacSensor8->sensor->listener = this;
	pacSensor8->value = Sensor::PAC_MAN;
	pacSensor8->isActive = false;
	sensors.add(pacSensor8);

	Sensor* pointButtonSensor = new Sensor;
	pointButtonSensor->sensor = App->physics->CreateRectangleSensor(90, 166, 16, 16, b2_staticBody);
	pointButtonSensor->sensor->listener = this;
	pointButtonSensor->value = Sensor::POINT_BUTTON;
	pointButtonSensor->isActive = false;
	sensors.add(pointButtonSensor);

	Sensor* safetyButtonSensor1 = new Sensor;
	safetyButtonSensor1->sensor = App->physics->CreateRectangleSensor(21, 814, 22, 12, b2_staticBody);
	safetyButtonSensor1->sensor->listener = this;
	safetyButtonSensor1->value = Sensor::SAFETY_BUTTON;
	safetyButtonSensor1->isActive = true;
	sensors.add(safetyButtonSensor1);

	Sensor* safetyButtonSensor2 = new Sensor;
	safetyButtonSensor2->sensor = App->physics->CreateRectangleSensor(283, 814, 22, 12, b2_staticBody);
	safetyButtonSensor2->sensor->listener = this;
	safetyButtonSensor2->value = Sensor::SAFETY_BUTTON;
	safetyButtonSensor2->isActive = true;
	sensors.add(safetyButtonSensor2);

	Sensor* numberButtonSensor = new Sensor;
	numberButtonSensor->sensor = App->physics->CreateRectangleSensor(28, 592, 4, 14, b2_staticBody);
	numberButtonSensor->sensor->listener = this;
	numberButtonSensor->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor->isActive = true;
	sensors.add(numberButtonSensor);

	Sensor* numberButtonSensor2 = new Sensor;
	numberButtonSensor2->sensor = App->physics->CreateRectangleSensor(28, 609, 4, 14, b2_staticBody);
	numberButtonSensor2->sensor->listener = this;
	numberButtonSensor2->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor2->isActive = true;
	sensors.add(numberButtonSensor2);

	Sensor* numberButtonSensor3 = new Sensor;
	numberButtonSensor3->sensor = App->physics->CreateRectangleSensor(28, 624, 4, 14, b2_staticBody);
	numberButtonSensor3->sensor->listener = this;
	numberButtonSensor3->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor3->isActive = true;
	sensors.add(numberButtonSensor3);

	Sensor* numberButtonSensor4 = new Sensor;
	numberButtonSensor4->sensor = App->physics->CreateRectangleSensor(28, 640, 4, 14, b2_staticBody);
	numberButtonSensor4->sensor->listener = this;
	numberButtonSensor4->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor4->isActive = true;
	sensors.add(numberButtonSensor4);

	Sensor* numberButtonSensor5 = new Sensor;
	numberButtonSensor5->sensor = App->physics->CreateRectangleSensor(28, 656, 4, 14, b2_staticBody);
	numberButtonSensor5->sensor->listener = this;
	numberButtonSensor5->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor5->isActive = true;
	sensors.add(numberButtonSensor5);

	Sensor* numberButtonSensor6 = new Sensor;
	numberButtonSensor6->sensor = App->physics->CreateRectangleSensor(28, 672, 4, 14, b2_staticBody);
	numberButtonSensor6->sensor->listener = this;
	numberButtonSensor6->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor6->isActive = true;
	sensors.add(numberButtonSensor6);

	Sensor* numberButtonSensor7 = new Sensor;
	numberButtonSensor7->sensor = App->physics->CreateRectangleSensor(28, 688, 4, 14, b2_staticBody);
	numberButtonSensor7->sensor->listener = this;
	numberButtonSensor7->value = Sensor::NUMBER_BUTTON;
	numberButtonSensor7->isActive = true;
	sensors.add(numberButtonSensor7);

	// BACKGROUND -------------------------------------------------------------------------------------
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
	backgrounds.add(App->physics->CreateChain(0, 0, BottomLeftWhite, 16, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, BottomRightWhite, 16, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));

	sideBumpy.add(App->physics->CreateChain(0, 0, BottomLeftPink, 12, b2_staticBody));
	sideBumpy.getLast()->data->listener = this;
	sideBumpy.add(App->physics->CreateChain(0, 0, BottomRightPink, 12, b2_staticBody));
	sideBumpy.getLast()->data->listener = this;

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

	eggAnim.PushBack({ 107,7,18,26 });
	eggAnim.PushBack({ 128,7,18,26 });
	eggAnim.PushBack({ 153,7,18,26 });
	eggAnim.loop = true;

	eggAnim2.PushBack({ 107,7,18,26 });
	eggAnim2.PushBack({ 128,7,18,26 });
	eggAnim2.PushBack({ 153,7,18,26 });
	eggAnim2.loop = true;

	eggAnim3.PushBack({ 107,7,18,26 });
	eggAnim3.PushBack({ 128,7,18,26 });
	eggAnim3.PushBack({ 153,7,18,26 });
	eggAnim3.loop = true;

	pointCounter = 100;
	pacBool = true;
	cardBool = true;
	executeOrderSafety = false;

	// TITLE SCREEN //
	currentScene = TITLE_SCREEN;
	backgroundTexture = App->textures->Load("pinball/titleScreen.png");
	App->audio->PlayMusic("pinball/audio/music/TitleScreen.ogg", 0);
	startTitle = false;

	// GAME OVER //
	gameOverFont = App->fonts->Load("pinball/bignesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);

	// BONUS //
	bonusBool = true;

	// EXIT //
	exitBool = true;
	createExit = false;
	exitBody = App->physics->CreateRectangle(exitRectSect.x + exitRectSect.w * 3 / 2, exitRectSect.y + exitRectSect.h / 2, exitRectSect.w, exitRectSect.h, b2_staticBody);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(background);
	App->fonts->Unload(font);

	//TITLE SCREEN
	App->textures->Unload(backgroundTexture);

	//GAME OVER
	App->fonts->Unload(gameOverFont);

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


	if (App->player->ballCount < 0 && currentScene != GAME_OVER)
	{
		currentScene = GAME_OVER;
	}

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	switch (currentScene)
	{
	case TITLE_SCREEN:
		if (startTitle)
		{
			startTitle = false;
			App->audio->PlayMusic("pinball/audio/music/TitleScreen.ogg", 0.0f);
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN) {
			currentScene = PINBALL;
			App->audio->PlayMusic("pinball/audio/music/silence.ogg");
		}

		App->renderer->Blit(backgroundTexture, 0, 0, true);
		break;

	case PINBALL:
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->physics->debug)
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

			// Bonus counter
			int bonusCounter = 0;
			SDL_Rect bonusSect = { 700,0,336,954 };
			p2List_item<Sensor*>* bonus = sensors.getFirst();
			while (bonus != NULL)
			{
				if (bonus->data->isActive == true && bonus->data->value == Sensor::CARD) {
					bonusCounter++;
					if (bonusCounter == 5) {
						App->renderer->Blit(background, 0, 0, true, &bonusSect);
						if (bonusBool) {
							bonusBool = false;
							App->audio->PlayMusic("pinball/audio/music/bonus.ogg", 0.0f, 0);
							bonusBody = App->physics->CreateCircle(154, 897, 12, b2_staticBody);
						}
						int x, y;
						bonusBody->GetPosition(x, y);
						App->renderer->Blit(background, x, y, false, &ballSect);
					}
				}
				bonus = bonus->next;
			}

			// Exit counter
			int exitCounter = 0;
			p2List_item<Sensor*>* exit = sensors.getFirst();
			while (exit != NULL)
			{
				if (exit->data->isActive == false && exit->data->value == Sensor::NUMBER_BUTTON) {
					exitCounter++;
					if (exitCounter == 7) {
						App->renderer->Blit(background, exitRectSect.x + exitRectSect.w, exitRectSect.y, false, &exitRectSect);
						if (exitBool) {
							exitBool = false;
							App->player->currentScore += 1000;
							exitBody->pendingToDelete = true;
						}
						App->renderer->Blit(App->player->playerText, 232, 599, false, &exitSect);
					}
				}
				exit = exit->next;
			}

			// Pac man counter
			int pacCounter = 0;
			p2List_item<Sensor*>* pac = sensors.getFirst();
			while (pac != NULL)
			{
				if (pac->data->isActive == true && pac->data->value == Sensor::PAC_MAN) {
					pacCounter++;
					if (pacCounter == 8 && pacBool) {
						pacBool = false;
						App->player->currentScore += 2000;
						pointCounter = 100;
					}
				}
				pac = pac->next;
			}

			// Card counter
			int cardCounter = 0;
			p2List_item<Sensor*>* card = sensors.getFirst();
			while (card != NULL)
			{
				if (card->data->isActive == true && card->data->value == Sensor::CARD) {
					cardCounter++;
					if (cardCounter == 5 && cardBool) {
						cardBool = false;
						App->player->currentScore += 5000;
					}
				}
				card = card->next;
			}

			int x, y;
			App->player->kicker.mobile->GetPosition(x, y);
			App->renderer->Blit(background, x, y - 88, false, &App->player->kickerSect);

			p2List_item<Bumper*>* b = bumpers.getFirst();
			while (b != NULL)
			{
				int x, y;
				b->data->bumpy->GetPosition(x, y);
				App->renderer->Blit(App->player->playerText, x - 2, y - 4, false, &b->data->animation.GetCurrentFrame(), false, 1.0f, b->data->bumpy->GetRotation());
				b = b->next;
			}

			p2List_item<Sensor*>* s = sensors.getFirst();
			while (s != NULL)
			{
				int x, y;
				s->data->sensor->GetPosition(x, y);
				switch (s->data->value)
				{
				case Sensor::CARD:
					if (s->data->isActive == true) {
						if (x == 77) {
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect.h, false, &cardSect, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (x == 109) {
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect2.h, false, &cardSect2, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (x == 141) {
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect3.h, false, &cardSect3, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (x == 173) {
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect4.h, false, &cardSect4, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (x == 205) {
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect5.h, false, &cardSect5, false, 1.0f, s->data->sensor->GetRotation());
						}
					}
					break;
				case Sensor::EGG:
					if (s->data->isActive == true) {
						if (x == 101) {
							App->renderer->Blit(App->player->playerText, x, y, false, &eggAnim.GetCurrentFrame(), false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (x == 141) {
							App->renderer->Blit(App->player->playerText, x, y, false, &eggAnim2.GetCurrentFrame(), false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (x == 181) {
							App->renderer->Blit(App->player->playerText, x, y, false, &eggAnim3.GetCurrentFrame(), false, 1.0f, s->data->sensor->GetRotation());
						}
					}
					break;
				case Sensor::PAC_MAN:
					if (s->data->isActive == true)
					{
						App->renderer->Blit(App->player->playerText, x + pacSect.w / 2 + 1, y + pacSect.h / 2, false, &pacSect, false, 1.0f, s->data->sensor->GetRotation());
					}
					break;
				case Sensor::POINT_BUTTON:
					App->renderer->Blit(App->player->playerText, x + pointButtonSect.w / 2, y + pointButtonSect.h / 2, false, &pointButtonSect, false, 1.0f, s->data->sensor->GetRotation());
					break;
				case Sensor::SAFETY_BUTTON:
					if (s->data->isActive == true && executeOrderSafety == true)
					{
						App->renderer->Blit(App->player->playerText, x + safetyButtonSect.w / 2, y + safetyButtonSect.h / 2, false, &safetyButtonSect, false, 1.0f, s->data->sensor->GetRotation());
					}
					break;
				case Sensor::NUMBER_BUTTON:
					if (s->data->isActive == true) {
						if (y == 578) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect.w / 2 - 13, y + numberButtonSect.h / 2, false, &numberButtonSect, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (y == 594) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect2.w / 2 - 13, y + numberButtonSect2.h / 2, false, &numberButtonSect2, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (y == 610) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect3.w / 2 - 13, y + numberButtonSect3.h / 2, false, &numberButtonSect3, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (y == 625) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect4.w / 2 - 13, y + numberButtonSect4.h / 2 + 1, false, &numberButtonSect4, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (y == 642) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect5.w / 2 - 13, y + numberButtonSect5.h / 2, false, &numberButtonSect5, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (y == 658) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect6.w / 2 - 13, y + numberButtonSect6.h / 2, false, &numberButtonSect6, false, 1.0f, s->data->sensor->GetRotation());
						}
						else if (y == 673) {
							App->renderer->Blit(App->player->playerText, x + numberButtonSect7.w / 2 - 13, y + numberButtonSect7.h / 2 + 1, false, &numberButtonSect7, false, 1.0f, s->data->sensor->GetRotation());
						}
					}
					break;
				default:
					break;
				}
				s = s->next;
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

			sprintf_s(pointCounterNum, 8, "%4d", pointCounter);
			App->fonts->BlitText(56 - fontSize/2, 130, font, pointCounterNum);

			// ray -----------------
			if (ray_on == true)
			{
				fVector destination(mouse.x - ray.x, mouse.y - ray.y);
				destination.Normalize();
				destination *= ray_hit;

				App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

				if (normal.x != 0.0f)
					App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
			}
		}
		break;

	case GAME_OVER:
		App->physics->debug = false;
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN)
		{
			currentScene = TITLE_SCREEN;
			bumperTimer = 0;
			App->player->ballCount = 2;
			startTitle = true;
		}

		sprintf_s(gameOverText, 10, "GAME OVER");
		App->fonts->BlitText(gameOverFontSize * 0.75, gameOverFontSize * 6, gameOverFont, gameOverText);

		sprintf_s(prevScore, 8, "P-SCORE");
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 9.5, gameOverFont, prevScore);
		sprintf_s(prevScoreNum, 12, "%6d", App->player->previousScore);
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 10.5, gameOverFont, prevScoreNum);

		sprintf_s(currentScore, 6, "SCORE");
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 12.5, gameOverFont, currentScore);
		sprintf_s(currentScoreNum, 12, "%6d", App->player->currentScore);
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 13.5, gameOverFont, currentScoreNum);

		sprintf_s(highScore, 8, "H-SCORE");
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 15.5, gameOverFont, highScore);
		sprintf_s(highScoreNum, 12, "%6d", App->player->highScore);
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 16.5, gameOverFont, highScoreNum);

		sprintf_s(spaceToContinue, 28, "- Press enter to continue -");
		App->fonts->BlitText(gameOverFontSize * 0.20, gameOverFontSize * 19, font, spaceToContinue);
		break;

	default:
		return UPDATE_ERROR;
		break;
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//This will be a fUN function in the future :)
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

	srand(time(NULL));
	int r = (rand() % 100) / 50;
	if (bodyA == sideBumpy.getFirst()->data && bodyB->listener == (Module*)App->player)
	{
		App->audio->PlayFx(sideBumperFx);
		bodyB->body->ApplyLinearImpulse(b2Vec2(r, -r), bodyB->body->GetWorldCenter(), true);
		App->player->currentScore += 10;
		return;
	}
	if (bodyA == sideBumpy.getLast()->data && bodyB->listener == (Module*)App->player)
	{
		App->audio->PlayFx(sideBumperFx);
		bodyB->body->ApplyLinearImpulse(b2Vec2(-r, -r), bodyB->body->GetWorldCenter(), true);
		App->player->currentScore += 10;
		return;
	}

	p2List_item<Sensor*>* s = sensors.getFirst();
	while (s != NULL)
	{
		if (bodyA == s->data->sensor && bodyB->listener == (Module*)App->player)
		{
			p2List_item<Sensor*>* reset;
			int x, y;
			b2Vec2 originTP;
			b2Vec2 forceTP;
			switch (s->data->value)
			{
			case Sensor::DEATH:
				App->audio->PlayFx(fallFx);
				App->player->isDead = true;
				executeOrderSafety = false;
				pointCounter = 100;
				reset = sensors.getFirst();
				while (reset != NULL)
				{
					if (reset->data->value == Sensor::PAC_MAN) {
						reset->data->isActive = false;
					}
					if (reset->data->value == Sensor::CARD) {
						reset->data->isActive = false;
					}
					if (reset->data->value == Sensor::EGG) {
						reset->data->isActive = false;
					}
					if (reset->data->value == Sensor::SAFETY_BUTTON) {
						reset->data->isActive = true;
					}
					if (reset->data->value == Sensor::NUMBER_BUTTON) {
						reset->data->isActive = true;
					}
					reset = reset->next;
				}
				if (bonusBody != nullptr)
				{
					bonusBool = true;
					bonusBody->pendingToDelete = true;
				}
				if (exitBody == nullptr)
				{
					exitBool = true;
					createExit = true;
				}
				// should ded the balls
				return;
			case Sensor::CARD:
				if (s->data->isActive == false) {
					s->data->isActive = true;
					App->player->currentScore += 500;
				}
				// should card the card
				return;
			case Sensor::EGG:
			{
				int eggCounter = 0;
				s->data->isActive = true;
				if (s->data->sensorTimer == 10)
				{
					s->data->sensorTimer = 0;
					s->data->sensor->GetPosition(x, y);
					if (x == 101) {
						eggAnim.Update();
					}
					if (x == 141) {
						eggAnim2.Update();
					}
					if (x == 181) {
						eggAnim3.Update();
					}
				}
				else
				{
					s->data->sensorTimer++;
				}

				if ((int)eggAnim.currentFrame == 2) {
					eggCounter++;
				}
				if ((int)eggAnim2.currentFrame == 2) {
					eggCounter++;
				}
				if ((int)eggAnim3.currentFrame == 2) {
					eggCounter++;
				}
				if (eggCounter == 3) {
					executeOrderSafety = true;
				}
				// should egg the eggg
				return;
			}
			case Sensor::HUNDREDS:
				if (s->data->sensorTimer == 10)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += 500;
				}
				else
				{
					s->data->sensorTimer++;
				}
				// should money your points
				return;
			case Sensor::THOUSAND:
				if (s->data->sensorTimer == 10)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += 1000;
				}
				else
				{
					s->data->sensorTimer++;
				}
				// should money your points a lot more
				return;
			case Sensor::TP:
				bodyB->body->SetTransform(b2Vec2((float)280, (float)360), 0.0f);
				originTP.x = 282;
				originTP.y = 362;
				forceTP.x = bodyB->body->GetWorldCenter().x - originTP.x;
				forceTP.y = bodyB->body->GetWorldCenter().y - originTP.y;
				forceTP *= 3;
				bodyB->body->ApplyLinearImpulse(forceTP, bodyB->body->GetWorldCenter(), true);
				// should tp to 280, 360 and force ball to left-top
				return;
			case Sensor::PAC_MAN:
				if (s->data->isActive == false)
				{
					App->audio->PlayFx(wallPacFx);
					s->data->isActive = true;
					App->player->currentScore += 100;
				}
				// should pac the man
				return;
			case Sensor::POINT_BUTTON:
				if (s->data->sensorTimer == 15)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += pointCounter;
					App->audio->PlayFx(buttonFx);
					if (pointCounter < 1000)
					{
						pointCounter += 100;
					}
				}
				else
				{
					s->data->sensorTimer++;
				}
				// should point the button
				return;
			case Sensor::SAFETY_BUTTON:
				if (s->data->isActive == true && executeOrderSafety == true)
				{
					b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
					force *= 6;
					bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);
					App->audio->PlayFx(buttonFx);
					s->data->isActive = false;
				}
				// should safety the button
			case Sensor::NUMBER_BUTTON:
				if (s->data->isActive == true)
				{
					App->player->currentScore += 100;
					App->audio->PlayFx(buttonFx);
					s->data->isActive = false;
				}
				// should number the button
				return;
			default:
				// should never happen
				return;
			}
		}
		s = s->next;
	}
}