#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

struct Bumper {
	PhysBody* bumpy;
	Animation animation;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = false);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> backgrounds;
	p2List<Bumper*> bumpers;
	int bumperTimer = 0;

	PhysBody* deathSensor;
	bool sensed;

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
		233, 82,
		252, 82,
		274, 93,
		284, 105,
		292, 119,
		300, 145,
		300, 586,
		282, 586,
		282, 570,
		296, 546,
		296, 528,
		290, 520,
		264, 520,
		250, 491,
		250, 415,
		234, 400,
		234, 384,
		256, 364,
		272, 377,
		287, 377,
		295, 368,
		295, 349,
		266, 320,
		266, 294,
		279, 287,
		286, 276,
		292, 263,
		292, 166,
		281, 141,
		270, 134,
		259, 128,
		236, 128,
		225, 133,
		216, 142,
		200, 142,
		200, 114
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

	// Font stuff
	int font = -1;
	int fontSize = 12;
	char test[16] = { "\0" };

	char currentScore[6] = { "\0" };
	char currentScoreNum[12] = { "\0" };

	char highScore[8] = { "\0" };
	char highScoreNum[12] = { "\0" };

	char prevScore[8] = { "\0" };
	char prevScoreNum[12] = { "\0" };

	char balls[6] = { "\0" };
	char ballsNum[2] = { "\0" };

};