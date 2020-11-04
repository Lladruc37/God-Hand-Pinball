#include "ModuleTransition.h"

#include "Application.h"

#include "ModuleRender.h"

#include "SDL/include/SDL_render.h"

ModuleTransition::ModuleTransition(Application* app, bool start_enabled) : Module(app, start_enabled) {
    screenRect = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

ModuleTransition::~ModuleTransition() {}

bool ModuleTransition::Start() {
    LOG("Preparing Fade Screen");

    // Enable blending mode for transparency
    SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);
    return true;
}

update_status ModuleTransition::Update() {
    // Exit this function if we are not performing a fade
    if (currentStep == Transition_Step::NONE) { return update_status::UPDATE_CONTINUE; }

    if (currentStep == Transition_Step::TO_BLACK)
    {
        ++frameCount;
        if (frameCount >= maxFadeFrames)
        {
            moduleToDisable->Disable();
            moduleToEnable->Enable();

            currentStep = Transition_Step::FROM_BLACK;
        }
    }
    else
    {
        --frameCount;
        if (frameCount <= 0)
        {
            currentStep = Transition_Step::NONE;
        }
    }

    return update_status::UPDATE_CONTINUE;
}

update_status ModuleTransition::PostUpdate() {
    // Exit this function if we are not performing a fade
    if (currentStep == Transition_Step::NONE) { return update_status::UPDATE_CONTINUE; }

    float fadeRatio = ((float)frameCount / (float)maxFadeFrames);

    // Render the black square with alpha on the screen
    SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
    SDL_RenderFillRect(App->renderer->renderer, &screenRect);

    return update_status::UPDATE_CONTINUE;
}

bool ModuleTransition::Transition(Module* moduleToDisable, Module* moduleToEnable, float frames) {
    // If we are already in a fade process, ignore this call
    if (currentStep == Transition_Step::NONE)
    {
        currentStep = Transition_Step::TO_BLACK;
        frameCount = 0;
        maxFadeFrames = frames;

        this->moduleToDisable = moduleToDisable;
        this->moduleToEnable = moduleToEnable;

        return true;
    }

    return false;
}