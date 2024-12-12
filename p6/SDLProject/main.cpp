/**
* Author: Sahil Singh
* Assignment: Not Platformer
* Date due: 11 Dec  5:00 pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <SDL_mixer.h>
#include "stb_image.h"
#include "Util.h"
#include "Entity.h"
#include <vector>
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "LevelMenu.h"
GLuint fontTextureID;
Mix_Music *music;
Mix_Chunk *bounce;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool paused = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[5];
void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Project 6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    glUseProgram(program.programID);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    fontTextureID = Util::LoadTexture("font1.png");
    sceneList[0] = new LevelMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level4();
    SwitchToScene(sceneList[0]);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("neon-laser.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME/10);
    Mix_PlayMusic(music, -1);
    bounce = Mix_LoadWAV("coin.wav");
}

void ProcessInput() {
    currentScene->state.player->movement = glm::vec3(0);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_SPACE:
                        break;
                    case SDLK_RETURN:
                        currentScene->state.enterPressed = true;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
       
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    
    else if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }
    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return; }
    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
       
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
    if (currentScene->state.player->collidedWithCoin == true){
        Mix_PlayChannel(-1, bounce, 0);
        currentScene->state.player->collidedWithCoin = false;
    }
    
    
    viewMatrix = glm::mat4(1.0f);

        if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 14)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        } else if (currentScene->state.player->position.x <= 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        
        else if (currentScene->state.player->position.x >= 14) {
            
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-14, 3.75, 0));
        }
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);
    SDL_GL_SwapWindow(displayWindow);
}
void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        if (currentScene->state.paused == false){
            Update();
        }
        if (currentScene->state.nextScene >= 0){
            int livesToPassOver  = currentScene->state.player->lives;
            SwitchToScene(sceneList[currentScene->state.nextScene]); //scene switch happens
            currentScene->state.player->lives =livesToPassOver;
        }
        Render();
    }
    
    Shutdown();
    return 0;
}
