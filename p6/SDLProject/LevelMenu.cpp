#include "LevelMenu.h"


unsigned int LEVELMENU_data[] = {};

GLuint fontTextureID5;

void LevelMenu::Initialize() {
    state.nextScene = -1;
    fontTextureID5=Util::LoadTexture("font1.png");
    state.player = new Entity();
    state.player->isActive = false;
    
}

void LevelMenu::Update(float deltaTime) {
    if (state.enterPressed == true) {
        state.nextScene = 1;
    }
}

void LevelMenu::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID5, "Not Platformer", 1.1f, -0.55f, glm::vec3(1.8f, -1.5f, 0.0f));
    Util::DrawText(program,  fontTextureID5, "Press Enter to start", 0.8f, -0.45f, glm:: vec3(1.9, -5.5,0.0f));


}
