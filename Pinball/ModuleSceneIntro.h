#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> backgrounds;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int backgroundChain[90] = {
		296, 861,
		203, 954,
		336, 954,
		336, 0,
		0, 0,
		0, 954,
		101, 954,
		6, 859,
		6, 745,
		37, 714,
		22, 699,
		22, 569,
		6, 520,
		6, 351,
		38, 319,
		38, 254,
		8, 164,
		9, 128,
		13, 114,
		18, 101,
		25, 88,
		32, 78,
		40, 69,
		49, 62,
		61, 56,
		72, 50,
		79, 48,
		256, 48,
		265, 51,
		277, 56,
		284, 60,
		294, 67,
		301, 74,
		308, 84,
		317, 98,
		322, 110,
		326, 121,
		330, 131,
		330, 794,
		300, 794,
		300, 634,
		282, 634,
		282, 699,
		267, 714,
		296, 743
	};
	int TopLeftBlue[22] = {
		76, 87,
		104, 115,
		104, 142,
		70, 175,
		70, 200,
		66, 206,
		60, 206,
		51, 188,
		42, 164,
		42, 127,
		54, 103
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
};
