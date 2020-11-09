#include "ModuleGameOver.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleTransition.h"
#include "ModuleTitleScreen.h"

#include "SDL/include/SDL_scancode.h"

ModuleGameOver::ModuleGameOver(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGameOver::~ModuleGameOver()
{}

// Load assets
bool ModuleGameOver::Start()
{
    LOG("Loading background assets");

	font = App->fonts->Load("pinball/nesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);
	gameOverFont = App->fonts->Load("pinball/bignesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);

    return true;
}

update_status ModuleGameOver::Update()
{
    if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN))
	{
        App->transition->Transition(this, (Module*)App->title_screen, 20.0f);
    }

    return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleGameOver::PostUpdate()
{
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

    return update_status::UPDATE_CONTINUE;
}

bool ModuleGameOver::CleanUp()
{
	App->fonts->Unload(font);
	App->fonts->Unload(gameOverFont);
    return true;
}