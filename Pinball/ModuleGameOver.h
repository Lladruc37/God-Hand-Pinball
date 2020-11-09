#include "Module.h"

struct SDL_Texture;

class ModuleGameOver : public Module {
public:
    //Constructor
    ModuleGameOver(Application* app, bool start_enabled = false);

    //Destructor
    ~ModuleGameOver();

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
    int font = -1;
    int gameOverFont = -1;
    int gameOverFontSize = 32;
    char gameOverText[10] = { "\0" };
    char spaceToContinue[28] = { "\0" };

    char currentScore[6] = { "\0" };
    char currentScoreNum[12] = { "\0" };

    char highScore[8] = { "\0" };
    char highScoreNum[12] = { "\0" };

    char prevScore[8] = { "\0" };
    char prevScoreNum[12] = { "\0" };

    char balls[6] = { "\0" };
    char ballsNum[2] = { "\0" };

};