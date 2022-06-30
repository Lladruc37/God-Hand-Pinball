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

	background = App->textures->Load("Game/pinball/Background.png");
	font = App->fonts->Load("Game/pinball/nesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);

	bumperFx = App->audio->LoadFx("Game/pinball/audio/fx/Bumper.wav");
	fallFx = App->audio->LoadFx("Game/pinball/audio/fx/PeachFall.wav");
	wallPacFx = App->audio->LoadFx("Game/pinball/audio/fx/WallButton_PacMan.wav");
	buttonFx = App->audio->LoadFx("Game/pinball/audio/fx/PointButton_SafetyButton.wav");
	sideBumperFx = App->audio->LoadFx("Game/pinball/audio/fx/SideBumpers.wav");
	pathsFx = App->audio->LoadFx("Game/pinball/audio/fx/Paths.wav");
	holeSafetyFx = App->audio->LoadFx("Game/pinball/audio/fx/Hole_SpawnSafety.wav");
	eggFx = App->audio->LoadFx("Game/pinball/audio/fx/PinkPlatform_BreakOpenEgg.wav");

	// Sensors
	CreateSensor(App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 10, b2_staticBody), Sensor::DEATH, false);
	CreateSensor(App->physics->CreateRectangleSensor(120, 120, 10, 10, b2_staticBody), Sensor::HUNDREDS, false);
	CreateSensor(App->physics->CreateRectangleSensor(152, 120, 10, 10, b2_staticBody), Sensor::THOUSAND, false);
	CreateSensor(App->physics->CreateRectangleSensor(184, 120, 10, 10, b2_staticBody), Sensor::HUNDREDS, false);
	CreateSensor(App->physics->CreateRectangleSensor(274, 200, 25, 11, b2_staticBody), Sensor::HUNDREDS, false);
	CreateSensor(App->physics->CreateRectangleSensor(87, 580, 10, 10, b2_staticBody), Sensor::CARD, false);
	CreateSensor(App->physics->CreateRectangleSensor(120, 580, 10, 10, b2_staticBody), Sensor::CARD, false);
	CreateSensor(App->physics->CreateRectangleSensor(151, 580, 10, 10, b2_staticBody), Sensor::CARD, false);
	CreateSensor(App->physics->CreateRectangleSensor(183, 580, 10, 10, b2_staticBody), Sensor::CARD, false);
	CreateSensor(App->physics->CreateRectangleSensor(216, 580, 10, 10, b2_staticBody), Sensor::CARD, false);
	CreateSensor(App->physics->CreateRectangleSensor(111, 783, 10, 10, b2_staticBody), Sensor::EGG, false);
	CreateSensor(App->physics->CreateRectangleSensor(151, 783, 10, 10, b2_staticBody), Sensor::EGG, false);
	CreateSensor(App->physics->CreateRectangleSensor(191, 783, 10, 10, b2_staticBody), Sensor::EGG, false);
	CreateSensor(App->physics->CreateRectangleSensor(47, 87, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(36, 101, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(28, 117, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(24, 135, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(24, 153, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(26, 171, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(32, 187, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(38, 203, 10, 10, b2_staticBody), Sensor::PAC_MAN, false);
	CreateSensor(App->physics->CreateRectangleSensor(90, 166, 14, 14, b2_staticBody), Sensor::POINT_BUTTON, false);
	CreateSensor(App->physics->CreateRectangleSensor(21, 814, 22, 12, b2_staticBody), Sensor::SAFETY_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(283, 814, 22, 12, b2_staticBody), Sensor::SAFETY_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 592, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 609, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 624, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 640, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 656, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 672, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(28, 688, 4, 14, b2_staticBody), Sensor::NUMBER_BUTTON, true);
	CreateSensor(App->physics->CreateRectangleSensor(280, 360, 15, 15, b2_staticBody), Sensor::TP, false);
	CreateSensor(App->physics->CreateRectangleSensor(280, 535, 15, 15, b2_staticBody), Sensor::TP, false);

	// BACKGROUND -------------------------------------------------------------------------------------
	backgrounds.add(App->physics->CreateChain(0, 0, backgroundChain, 76, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftBlue, 30, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopLeftSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopRightSmol, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopRightBig, 72, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, TopRightTunnelWall, 16, b2_staticBody));
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

	sealAnim.PushBack({ 124,154,32,32 });
	sealAnim.PushBack({ 158,154,32,32 });
	sealAnim.loop = true;
	sealAnim.speed = 0.175f;

	ballAnim.PushBack({ 160,126,14,16 });
	ballAnim.PushBack({ 180,126,14,16 });
	ballAnim.loop = true;
	ballAnim.speed = 0.175f;

	pointCounter = 100;
	pacBool = true;
	cardBool = true;
	executeOrderSafety = false;

	// TITLE SCREEN //
	currentScene = TITLE_SCREEN;
	backgroundTexture = App->textures->Load("Game/pinball/titleScreen.png");
	App->audio->PlayMusic("Game/pinball/audio/music/TitleScreen.ogg", 0);
	startTitle = false;

	// GAME OVER //
	gameOverFont = App->fonts->Load("Game/pinball/bignesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);

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
	srand(time(NULL));
	switch (currentScene)
	{
	case TITLE_SCREEN:
		if (startTitle)
		{
			startTitle = false;
			App->audio->PlayMusic("Game/pinball/audio/music/TitleScreen.ogg", 0.0f);
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN) {
			currentScene = PINBALL;
			App->audio->PlayMusic("Game/pinball/audio/music/silence.ogg");
		}

		App->renderer->Blit(backgroundTexture, 0, 0, true);
		break;

	case PINBALL:
		{
			p2List_item<Sensor*>* timer = sensors.getFirst();
			while (timer != NULL)
			{
				if (timer->data->sensorTimer < 100)
				{
					timer->data->sensorTimer++;
				}
				timer = timer->next;
			}

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
			p2List_item<Sensor*>* sensor = sensors.getFirst();
			while (sensor != NULL)
			{
				if (sensor->data->isActive == true && sensor->data->value == Sensor::CARD) {
					bonusCounter++;
					if (bonusCounter == 5) {
						App->renderer->Blit(background, 0, 0, true, &bonusSect);
						if (bonusBool) {
							bonusBool = false;
							App->audio->PlayMusic("Game/pinball/audio/music/bonus.ogg", 0.0f, 0);
							bonusBody = App->physics->CreateCircle(154, 897, 12, b2_staticBody);
						}
						int x, y;
						bonusBody->GetPosition(x, y);
						App->renderer->Blit(background, x, y, false, &ballSect);
					}
				}
				sensor = sensor->next;
			}

			// Seal blitting
			App->renderer->Blit(App->player->playerText, 88, 224, false, &sealAnim.GetCurrentFrame());
			App->renderer->Blit(App->player->playerText, 184, 224, false, &sealAnim.GetCurrentFrame(), true);
			App->renderer->Blit(App->player->playerText, 96, 206, false, &ballAnim.GetCurrentFrame());
			App->renderer->Blit(App->player->playerText, 194, 206, false, &ballAnim.GetCurrentFrame(), true);

			// Exit counter
			int exitCounter = 0;
			// Pac man counter
			int pacCounter = 0;
			// Card counter
			int cardCounter = 0;

			// Counting lots of stuf
			p2List_item<Sensor*>* sensor2 = sensors.getFirst();
			while (sensor2 != NULL)
			{
				if (sensor2->data->isActive == false && sensor2->data->value == Sensor::NUMBER_BUTTON) {
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

				if (sensor2->data->isActive == true && sensor2->data->value == Sensor::PAC_MAN) {
					pacCounter++;
					if (pacCounter == 8) {
						int currentLoopCount = sealAnim.loopCount;
						if (sealAnim.loopCount != 20)
						{
							sealAnim.Update();
							ballAnim.Update();
						}
						if (currentLoopCount != sealAnim.loopCount)
						{
							App->audio->PlayFx(bumperFx);
							bumpers.getFirst()->data->animation.Update();
							App->player->currentScore += 100;
						}
						if (pacBool)
						{
							pacBool = false;
							pointCounter = 100;
						}
					}
					if (sealAnim.loopCount == 20)
					{
						sensor2->data->isActive = false;
						pacBool = true;
						pacCounter--;
					}
				}

				if (sensor2->data->isActive == true && sensor2->data->value == Sensor::CARD) {
					cardCounter++;
					if (cardCounter == 5 && cardBool) {
						cardBool = false;
						App->player->currentScore += 5000;
					}
				}

				sensor2 = sensor2->next;
			}

			// Blitting kicker
			int kickerX, kickerY;
			App->player->kicker.mobile->GetPosition(kickerX, kickerY);
			if (bonusCounter == 5)
			{
				App->renderer->Blit(background, kickerX, kickerY - 88, false, &App->player->kickerSectBonus);
			}
			else
			{
				App->renderer->Blit(background, kickerX, kickerY - 88, false, &App->player->kickerSect);
			}

			// Blitting bumpers
			p2List_item<Bumper*>* b = bumpers.getFirst();
			while (b != NULL)
			{
				int x, y;
				b->data->bumpy->GetPosition(x, y);
				App->renderer->Blit(App->player->playerText, x - 2, y - 4, false, &b->data->animation.GetCurrentFrame(), false, 1.0f, b->data->bumpy->GetRotation());
				b = b->next;
			}

			// Blitting sensors
			p2List_item<Sensor*>* s = sensors.getFirst();
			while (s != NULL)
			{
				int x, y;
				s->data->sensor->GetPosition(x, y);
				switch (s->data->value)
				{
				case Sensor::CARD:
					if (s->data->isActive == true) {
						switch (x)
						{
						case 77:
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect.h, false, &cardSect, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 109:
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect2.h, false, &cardSect2, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 141:
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect3.h, false, &cardSect3, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 173:
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect4.h, false, &cardSect4, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 205:
							App->renderer->Blit(App->player->playerText, x - 2, y - cardSect5.h, false, &cardSect5, false, 1.0f, s->data->sensor->GetRotation());
							break;
						default:
							break;
						}
					}
					break;
				case Sensor::EGG:
					if (s->data->isActive == true) {
						switch (x)
						{
						case 101:
							App->renderer->Blit(App->player->playerText, x, y, false, &eggAnim.GetCurrentFrame(), false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 141:
							App->renderer->Blit(App->player->playerText, x, y, false, &eggAnim2.GetCurrentFrame(), false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 181:
							App->renderer->Blit(App->player->playerText, x, y, false, &eggAnim3.GetCurrentFrame(), false, 1.0f, s->data->sensor->GetRotation());
							break;
						default:
							break;
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
						switch (y)
						{
						case 578:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect.w / 2 - 13, y + numberButtonSect.h / 2, false, &numberButtonSect, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 594:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect2.w / 2 - 13, y + numberButtonSect2.h / 2, false, &numberButtonSect2, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 610:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect3.w / 2 - 13, y + numberButtonSect3.h / 2, false, &numberButtonSect3, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 625:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect4.w / 2 - 13, y + numberButtonSect4.h / 2 + 1, false, &numberButtonSect4, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 642:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect5.w / 2 - 13, y + numberButtonSect5.h / 2, false, &numberButtonSect5, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 658:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect6.w / 2 - 13, y + numberButtonSect6.h / 2, false, &numberButtonSect6, false, 1.0f, s->data->sensor->GetRotation());
							break;
						case 673:
							App->renderer->Blit(App->player->playerText, x + numberButtonSect7.w / 2 - 13, y + numberButtonSect7.h / 2 + 1, false, &numberButtonSect7, false, 1.0f, s->data->sensor->GetRotation());
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
				s = s->next;
			}

			if (pacCounter == 0)
			{
				sealAnim.Reset();
				ballAnim.Reset();
			}

			// Text UI ----------------
			App->fonts->BlitText(fontSize * 2, fontSize * 1, font, "P-SCORE");
			sprintf_s(prevScoreNum, 12, "%6d", App->player->previousScore);
			App->fonts->BlitText(fontSize * 2.5, fontSize * 2, font, prevScoreNum);

			App->fonts->BlitText(fontSize * 11, fontSize * 1, font, "SCORE");
			sprintf_s(currentScoreNum, 12, "%6d", App->player->currentScore);
			App->fonts->BlitText(fontSize * 10.5, fontSize * 2, font, currentScoreNum);

			App->fonts->BlitText(fontSize * 18, fontSize * 1, font, "H-SCORE");
			sprintf_s(highScoreNum, 12, "%6d", App->player->highScore);
			App->fonts->BlitText(fontSize * 18.5, fontSize * 2, font, highScoreNum);

			App->fonts->BlitText(fontSize * 19.5, fontSize * 77, font, "BALLS");
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

		App->fonts->BlitText(gameOverFontSize * 0.75, gameOverFontSize * 6, gameOverFont, "GAME OVER");

		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 9.5, gameOverFont, "P-SCORE");
		sprintf_s(prevScoreNum, 12, "%6d", App->player->previousScore);
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 10.5, gameOverFont, prevScoreNum);

		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 12.5, gameOverFont, "SCORE");
		sprintf_s(currentScoreNum, 12, "%6d", App->player->currentScore);
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 13.5, gameOverFont, currentScoreNum);

		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 15.5, gameOverFont, "H-SCORE");
		sprintf_s(highScoreNum, 12, "%6d", App->player->highScore);
		App->fonts->BlitText(gameOverFontSize * 1.75, gameOverFontSize * 16.5, gameOverFont, highScoreNum);

		App->fonts->BlitText(gameOverFontSize * 0.20, gameOverFontSize * 19, font, "- Press enter to continue -");
		break;

	default:
		return UPDATE_ERROR;
		break;
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// This will be a fUN function in the future :)
	// Bumpers
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

	// Side bumpers
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

	// Sensors
	p2List_item<Sensor*>* s = sensors.getFirst();
	while (s != NULL)
	{
		if (bodyA == s->data->sensor && bodyB->listener == (Module*)App->player)
		{
			switch (s->data->value)
			{
			case Sensor::DEATH:
				{
					p2List_item<Sensor*>* reset;
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
				}
			case Sensor::CARD:
				if (s->data->isActive == false) {
					s->data->isActive = true;
				}
				if (s->data->sensorTimer >= 40)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += 500;
					App->audio->PlayFx(pathsFx);
				}
				// should card the card
				return;
			case Sensor::EGG:
				{
					int x, y;
					int eggCounter = 0;
					s->data->isActive = true;
					if (s->data->sensorTimer >= 50)
					{
						App->audio->PlayFx(eggFx);
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
						App->audio->PlayFx(holeSafetyFx);
						executeOrderSafety = true;
					}
					// should egg the eggg
					return;
				}
			case Sensor::HUNDREDS:
				if (s->data->sensorTimer >= 30)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += 500;
					App->audio->PlayFx(pathsFx);
				}
				// should money your points
				return;
			case Sensor::THOUSAND:
				if (s->data->sensorTimer >= 30)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += 1000;
					App->audio->PlayFx(pathsFx);
				}
				// should money your points a lot more
				return;
			case Sensor::TP:
				{
					App->audio->PlayFx(holeSafetyFx);
					b2Vec2 originTP;
					originTP.x = 282;
					originTP.y = 362;
					b2Vec2 destinyTP = bodyB->body->GetPosition();
					destinyTP.x = METERS_TO_PIXELS(destinyTP.x);
					destinyTP.y = METERS_TO_PIXELS(destinyTP.y);
					if (destinyTP.y > originTP.y)
					{
						bodyB->body->ApplyLinearImpulse({10,-10000.0f}, bodyB->body->GetWorldCenter(), true);
					}
					else
					{
						bodyB->body->SetLinearVelocity(b2Vec2(0, 0));
						bodyB->body->SetAngularVelocity(0);
						bodyB->body->ApplyLinearImpulse(b2Vec2(-1.25,-1.25), bodyB->body->GetWorldCenter(), true);
					}
					return;
				}
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
				if (s->data->sensorTimer >= 50)
				{
					s->data->sensorTimer = 0;
					App->player->currentScore += pointCounter;
					App->audio->PlayFx(buttonFx);
					bodyB->body->ApplyLinearImpulse(b2Vec2(0.25,0.25), bodyB->body->GetWorldCenter(), true);
					if (pointCounter < 1000)
					{
						pointCounter += 100;
					}
				}
				// should point the button
				return;
			case Sensor::SAFETY_BUTTON:
			{
				if (s->data->isActive == true && executeOrderSafety == true)
				{
					b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
					force *= 6;
					bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);
					App->audio->PlayFx(buttonFx);
					s->data->isActive = false;
				}
				// should safety the button
			}
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
	// Yey!
}

void ModuleSceneIntro::CreateSensor(PhysBody* sensor, Sensor::sensorValue sensorType, bool isActive)
{
	Sensor* newSensor = new Sensor;
	newSensor->sensor = sensor;
	newSensor->sensor->listener = this;
	newSensor->value = sensorType;
	newSensor->isActive = isActive;
	sensors.add(newSensor);
}
