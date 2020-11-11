#pragma once
#include "Module.h"

struct SDL_Texture;

class ModuleTitleScreen : public Module {
public:
    //Constructor
    ModuleTitleScreen(Application* app, bool start_enabled = false);

    //Destructor
    ~ModuleTitleScreen();

    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start() override;

    // Called at the middle of the application loop
    // Updates the scene's background animations
    update_status Update() override;

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    update_status PostUpdate() override;

    bool CleanUp() override;

private:
    // The scene sprite sheet loaded into an SDL_Texture
    SDL_Texture* backgroundTexture = nullptr;
};