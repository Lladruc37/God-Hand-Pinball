#include "ModuleTitleScreen.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleTransition.h"
#include "ModuleSceneIntro.h"

#include "SDL/include/SDL_scancode.h"

ModuleTitleScreen::ModuleTitleScreen(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleTitleScreen::~ModuleTitleScreen()
{}

// Load assets
bool ModuleTitleScreen::Start()
{
    LOG("Loading background assets");

    backgroundTexture = App->textures->Load("pinball/titleScreen.png");
    App->audio->PlayMusic("pinball/audio/music/TitleScreen.ogg",0);

    return true;
}

update_status ModuleTitleScreen::Update()
{
    if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN)) {
        App->transition->Transition(this, (Module*)App->scene_intro, 20.0f);
    }

    return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleTitleScreen::PostUpdate()
{
    // Draw everything --------------------------------------
    App->renderer->Blit(backgroundTexture, 0, 0, true);

    return update_status::UPDATE_CONTINUE;
}

bool ModuleTitleScreen::CleanUp()
{
    App->textures->Unload(backgroundTexture);

    return true;
}