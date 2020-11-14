#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

enum Scene
{
	TITLE_SCREEN,
	PINBALL,
	GAME_OVER
};

class PhysBody;

struct Bumper {
	PhysBody* bumpy;
	Animation animation;
};

struct Sensor {
	enum sensorValue
	{
		NONE = -1,
		DEATH,
		CARD,
		EGG,
		TP,
		PAC_MAN,
		HUNDREDS,
		THOUSAND,
		POINT_BUTTON,
		SAFETY_BUTTON,
		NUMBER_BUTTON
	};
	PhysBody* sensor;
	sensorValue value;
	bool isActive;
	int sensorTimer = 0;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void CreateSensor(PhysBody* sensor, Sensor::sensorValue sensorType, bool isActive);

public:
	p2List<PhysBody*> backgrounds;
	p2List<Bumper*> bumpers;
	int bumperTimer = 0;
	p2List<PhysBody*> sideBumpy;
	p2List<Sensor*> sensors;
	SDL_Rect cardSect = { 197,149,26,46 };
	SDL_Rect cardSect2 = { 229,149,26,46 };
	SDL_Rect cardSect3 = { 261,149,26,46 };
	SDL_Rect cardSect4 = { 307,79,26,46 };
	SDL_Rect cardSect5 = { 339,79,26,46 };
	SDL_Rect pacSect = { 40,144,9,11 };
	SDL_Rect pointButtonSect = { 190,322,16,16 };
	SDL_Rect safetyButtonSect = { 156,324,22,12 };
	SDL_Rect numberButtonSect = { 4,206,30,14 };
	SDL_Rect numberButtonSect2 = { 4,222,30,14 };
	SDL_Rect numberButtonSect3 = { 4,238,30,14 };
	SDL_Rect numberButtonSect4 = { 4,254,30,14 };
	SDL_Rect numberButtonSect5 = { 4,270,30,14 };
	SDL_Rect numberButtonSect6 = { 4,286,30,14 };
	SDL_Rect numberButtonSect7 = { 4,302,30,14 };
	Animation cardAnim;
	Animation eggAnim;
	Animation eggAnim2;
	Animation eggAnim3;
	Animation sealAnim;
	Animation ballAnim;
	int pointCounter;
	bool pacBool;
	bool cardBool;
	bool executeOrderSafety;

	SDL_Texture* background;
	p2Point<int> ray;
	bool ray_on;

	// Chain Fun
	int backgroundChain[76] = {
	336, 954,
	203, 954,
	294, 863,
	294, 741,
	265, 713,
	280, 699,
	280, 634,
	300, 634,
	300, 890,
	328, 890,
	328, 140,
	324, 115,
	315, 93,
	305, 77,
	292, 64,
	276, 55,
	257, 48,
	77, 48,
	60, 55,
	44, 65,
	31, 78,
	20, 94,
	12, 113,
	8, 127,
	8, 165,
	38, 254,
	38, 319,
	6, 351,
	6, 520,
	22, 569,
	22, 700,
	37, 714,
	6, 745,
	6, 859,
	101, 954,
	0, 954,
	0, 0,
	336, 0
	};
	int TopLeftBlue[30] = {
		104, 141,
		70, 175,
		70, 199,
		66, 206,
		60, 206,
		52, 193,
		46, 177,
		42, 163,
		42, 126,
		48, 112,
		53, 103,
		60, 95,
		66, 90,
		75, 86,
		104, 115
	};
	int TopLeftSmol[8] = {
		136, 114,
		136, 132,
		138, 132,
		138, 114
	};
	int TopRightSmol[8] = {
		168, 114,
		168, 132,
		170, 132,
		170, 114
	};
	int TopRightBig[72] = {
	282, 586,
	300, 586,
	300, 147,
	295, 129,
	292, 119,
	283, 104,
	275, 94,
	263, 87,
	252, 82,
	233, 82,
	200, 115,
	200, 142,
	215, 142,
	223, 134,
	232, 130,
	242, 128,
	254, 128,
	264, 130,
	273, 134,
	283, 144,
	288, 155,
	292, 168,
	292, 262,
	288, 274,
	282, 283,
	275, 289,
	266, 294,
	266, 318,
	294, 346,
	296, 354,
	296, 366,
	293, 373,
	293, 523,
	296, 531,
	296, 544,
	282, 571
	};
	int TopRightTunnelWall[16] = {
	256, 363,
	270, 377,
	270, 520,
	264, 520,
	250, 492,
	250, 415,
	234, 399,
	234, 385
	};
	int TopRightGreen[22] = {
		237, 175,
		243, 171,
		249, 169,
		257, 172,
		262, 177,
		262, 248,
		258, 254,
		253, 254,
		248, 248,
		248, 188,
		237, 178
	};
	int MidLeft[20] = {
		45, 520,
		36, 506,
		36, 375,
		47, 365,
		50, 365,
		70, 385,
		70, 400,
		54, 415,
		54, 514,
		50, 520
	};
	int FirstCardPellet[8] = {
		72, 572,
		72, 590,
		74, 590,
		74, 572
	};
	int SecondCardPellet[8] = {
		104, 572,
		104, 590,
		106, 590,
		106, 572
	};
	int ThirdCardPellet[8] = {
		136, 572,
		136, 590,
		138, 590,
		138, 572
	};
	int FourthCardPellet[8] = {
		168, 572,
		168, 590,
		170, 590,
		170, 572
	};
	int FifthCardPellet[8] = {
		200, 572,
		200, 590,
		202, 590,
		202, 572
	};
	int SixthCardPellet[8] = {
		232, 572,
		232, 590,
		234, 590,
		234, 572
	};
	int BottomLeftPink[12] = {
		68, 766,
		68, 802,
		77, 811,
		83, 815,
		85, 813,
		70, 766
	};
	int BottomRightPink[12] = {
		234, 766,
		236, 766,
		236, 802,
		227, 811,
		221, 815,
		219, 813
	};
	int BottomLeftWhite[16] = {
		37, 763,
		37, 819,
		53, 837,
		71, 851,
		71, 847,
		51, 831,
		40, 818,
		40, 763
	};
	int BottomRightWhite[16] = {
		265, 763,
		267, 763,
		267, 819,
		256, 833,
		233, 851,
		233, 847,
		254, 831,
		265, 817
	};

	// Fx stuff
	uint bumperFx;
	uint fallFx;
	uint wallPacFx;
	uint buttonFx;
	uint sideBumperFx;
	uint pathsFx;
	uint holeSafetyFx;
	uint eggFx;

	// Font stuff
	int font = -1;
	int fontSize = 12;

	char currentScoreNum[12] = { "\0" };
	char highScoreNum[12] = { "\0" };
	char prevScoreNum[12] = { "\0" };
	char ballsNum[2] = { "\0" };
	char pointCounterNum[8] = { "\0" };

	Scene currentScene;

	// GAME OVER SCREEN //
	int gameOverFont = -1;
	int gameOverFontSize = 32;

	// TITLE SCREEN //
	SDL_Texture* backgroundTexture = nullptr;
	bool startTitle;

	// BONUS //
	bool bonusBool;
	PhysBody* bonusBody = nullptr;
	SDL_Rect ballSect = { 140,880,24,26 };

	// EXIT //
	bool exitBool;
	bool createExit;
	PhysBody* exitBody = nullptr;
	SDL_Rect exitSect = { 126,290,38,14 };
	SDL_Rect exitRectSect = { 256,588,22,44 };
};