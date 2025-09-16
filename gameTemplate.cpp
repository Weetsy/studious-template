/**
 * @file game.cpp
 * @author Alec Jackson, Christian Galvez
 * @brief game.cpp and game.hpp are example files demonstrating the use of the studious
 *       game engine. These two basic game files will generate a basic scene when the
 *       engine is compiled and ran.
 * @version 0.1
 * @date 2023-07-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <physics.hpp>
#include <OpenGlGfxController.hpp>
#include <AnimationController.hpp>
#include <GameInstance.hpp>

// Lists of embedded/core shaders
#ifndef GFX_EMBEDDED
vector<ProgramData> programs = {
    { "gameObject", "shaders/core/gameObject.vert", "shaders/core/gameObject.frag" },
    { "colliderObject", "shaders/core/colliderObject.vert", "shaders/core/colliderObject.frag" },
    { "textObject", "shaders/core/textObject.vert", "shaders/core/textObject.frag" },
    { "spriteObject", "shaders/core/spriteObject.vert", "shaders/core/spriteObject.frag" },
    { "uiObject", "shaders/core/uiObject.vert", "shaders/core/uiObject.frag" },
    { "tileObject", "shaders/core/tileObject.vert", "shaders/core/tileObject.frag" }
};
#else
vector<ProgramData> programs = {
    { "gameObject", "shaders/es/gameObject.vert", "shaders/es/gameObject.frag" },
    { "colliderObject", "shaders/es/colliderObject.vert", "shaders/es/colliderObject.frag" },
    { "textObject", "shaders/es/textObject.vert", "shaders/es/textObject.frag" },
    { "spriteObject", "shaders/es/spriteObject.vert", "shaders/es/spriteObject.frag" },
    { "uiObject", "shaders/es/uiObject.vert", "shaders/es/uiObject.frag" },
    { "tileObject", "shaders/es/tileObject.vert", "shaders/es/tileObject.frag" }
};
#endif

extern std::unique_ptr<GfxController> gfxController;
extern std::unique_ptr<AnimationController> animationController;
extern std::unique_ptr<PhysicsController> physicsController;

int runtime(GameInstance *currentGame);
int mainLoop(GameInstance *currentGame, CameraObject *currentCamera);

int main() {
    int errorNum;
    auto config = StudiousConfig("config.txt");

    GameInstance currentGame(config);
    currentGame.createGameScene("demo-scene");

    // Load shader programs
    for (auto program : programs) {
        gfxController.get()->loadShaders(program.programName, program.vertexShaderPath, program.fragmentShaderPath);
    }
    errorNum = runtime(&currentGame);
    return errorNum;
}

/*
 (int) runtime takes a (GameInstance *) gamein to create the current scene in.
 This function creates all of the GameObjects and CameraObjects in the current
 scene and creates a seperate thread for handling user input. All of the setup
 done in the runtime function is for demonstration purposes for now. The final
 studious engine product will source scene information from a .yaml file
 supplied by the user and build the game scene based on that data.

 (int) runtime returns 0 on success.
*/
int runtime(GameInstance *currentGame) {
    cout << "Building game scene!\n";
    SDL_SetRelativeMouseMode(SDL_FALSE);
    cout << "Creating camera.\n";

    auto currentCamera = currentGame->createCamera(nullptr, vec3(0), 0.0, 16.0 / 9.0, 4.0, 90.0, "mainCamera");
    /*
     End Scene Loading
     */
    mainLoop(currentGame, currentCamera);
    return 0;
}

/*
 (int) mainLoop starts rendering objects in the current GameInstance to the
 main SDL window. The methods called from the currentGame object render parts
 of the scene.

 (int) mainLoop returns 0 when closed successfully. When an error occurs and the
 mainLoop closes prematurely, an error code is returned.
*/
int mainLoop(GameInstance *currentGame, CameraObject *currentCamera) {
    Uint64 begin, end;
    double currentTime = 0.0, sampleTime = 1.0;
    int error = 0;
    vector<double> times;
    while (!currentGame->isShutDown()) {
        begin = SDL_GetPerformanceCounter();
        if (currentGame->pollInput(GameInput::QUIT)) currentGame->shutdown();
        error = currentGame->update();
        if (error) {
            return error;
        }
        end = SDL_GetPerformanceCounter();
        // Update player position
        deltaTime = static_cast<double>(end - begin) / (SDL_GetPerformanceFrequency());
        if (SHOW_FPS) {  // use sampleSize to find average FPS
            times.push_back(deltaTime);
            currentTime += deltaTime;
            if (currentTime > sampleTime) {
                currentTime = 0.0f;
                double sum = 0.0;
                vector<double>::iterator it;
                for (it = times.begin(); it != times.end(); ++it) sum += *it;
                sum /= times.size();
                times.clear();
                cout << "FPS: " << 1.0 / sum << '\n';
            }
        }
    }
    return 0;
}
