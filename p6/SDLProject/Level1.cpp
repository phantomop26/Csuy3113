#include "Level1.h"
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 8


#define LEVEL1_ENEMY_COUNT 3
#define LEVEL1_STAR_COUNT 9

GLuint fontTextureID2;

unsigned int level1_data[] ={
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,3,3,3,3,3,3,
    3, 0, 2, 2, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0,0,0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,0,0,0,0,0,3,
    3, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0,0,0,2,2,0,3,
    3, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0,0,0,2,2,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2,0,0,2,2,0,3,
    3, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,0,0,2,2,0,3,
    3, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,2,2,2,2,2,3,
};




void Level1::Initialize() {
    state.nextScene = -1; 
    
    fontTextureID2=Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tileset_new.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player = new Entity();
    state.player -> entityType = PLAYER;
    state.player->position = glm::vec3(1, -6,0);//player's start position
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("playerImg2.png");

    state.player->height=0.8;
    state.player->width=0.8;
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ufo.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position= glm::vec3(4.5, -3,0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].acceleration = glm::vec3(0,-3.0f,0);
    state.enemies[0].velocity = glm::vec3(0,1,0);
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position= glm::vec3(12.5, -4,0);
    state.enemies[2].aiType = DISAPPEARING;
    state.enemies[2].acceleration = glm::vec3(0,-3.0f,0);
    state.enemies[2].velocity = glm::vec3(0,1,0);
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position= glm::vec3(12.5, -4,0);
    state.enemies[1].aiType = MOVING_LEFT_RIGHT;
    state.enemies[1].acceleration = glm::vec3(0,-9.0f,0);
    state.enemies[1].velocity = glm::vec3(0,1,0);
    
    state.stars = new Entity[LEVEL1_STAR_COUNT];
    GLuint starTextureID = Util::LoadTexture("meteor_DetailedLarge.png");
    state.stars[0].entityType = STAR;
    state.stars[0].textureID = starTextureID;
    state.stars[0].position= glm::vec3(8,-3,0);
    
    state.stars[1].entityType = STAR;
    state.stars[1].textureID = starTextureID;
    state.stars[1].position= glm::vec3(1,-4,0);
    
    state.stars[2].entityType = STAR;
    state.stars[2].textureID = starTextureID;
    state.stars[2].position= glm::vec3(1,-1,0);//upper left corner
    
    state.stars[3].entityType = STAR;
    state.stars[3].textureID = starTextureID;
    state.stars[3].position= glm::vec3(3,-2,0);
    
    state.stars[3].entityType = STAR;
    state.stars[3].textureID = starTextureID;
    state.stars[3].position= glm::vec3(3,-5,0);
    
    state.stars[4].entityType = STAR;
    state.stars[4].textureID = starTextureID;
    state.stars[4].position= glm::vec3(3,-2,0);
    
    
    state.stars[5].entityType = STAR;
    state.stars[5].textureID = starTextureID;
    state.stars[5].position= glm::vec3(8,-1,0);
    
    state.stars[6].entityType = STAR;
    state.stars[6].textureID = starTextureID;
    state.stars[6].position= glm::vec3(11,-4,0);
    
    state.stars[7].entityType = STAR;
    state.stars[7].textureID = starTextureID;
    state.stars[7].position= glm::vec3(11,-1,0);
    
    state.stars[8].entityType = STAR;
    state.stars[8].textureID = starTextureID;
    state.stars[8].position= glm::vec3(18,-1,0);
    

    
    
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.stars, LEVEL1_STAR_COUNT);
    
    for (int i =0; i< LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.stars, LEVEL1_STAR_COUNT);
    }
    
    for (int i =0; i< LEVEL1_STAR_COUNT; i++) {
        state.stars[i].Update(deltaTime, state.player, state.enemies,  LEVEL1_ENEMY_COUNT, state.map, state.stars, LEVEL1_STAR_COUNT);
    }
    

    
    if (state.stars[0].isActive == false &&state.stars[1].isActive == false &&state.stars[2].isActive == false &&
        state.stars[3].isActive == false && state.stars[4].isActive == false && state.stars[5].isActive == false &&
        state.stars[6].isActive == false && state.stars[7].isActive == false && state.stars[8].isActive == false &&
        state.stars[9].isActive == false && state.stars[10].isActive == false){
        state.nextScene = 2;
    }
}


void Level1::Render(ShaderProgram *program) {

    
    state.map->Render(program);
    state.player->Render(program);
    
    if (state.player->position.x < 5){
        Util::DrawText(program, fontTextureID2, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(7.5, -0.5,0.0f));
    }
    
    else if (state.player->position.x >= 5 && state.player->position.x <14) {
        Util::DrawText(program, fontTextureID2, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(state.player->position.x+2.5, -0.5,0.0f));
    }
    
    else{
         Util::DrawText(program, fontTextureID2, "Lives:" + std::to_string(state.player->lives) , 0.5f, -0.2f, glm:: vec3(16.5, -0.5,0.0f));
    }
    
   
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++ ) {
        state.enemies[i].Render(program);
    }
    
    for (int i = 0; i < LEVEL1_STAR_COUNT; i++ ) {
        state.stars[i].Render(program);
    }
    
    if (state.player ->lives == 0){ 
        if (state.player->position.x >= 3 && state.player->position.x < 6){
            Util::DrawText(program, fontTextureID2, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(1.5, -2.5,0.0f));
        }
        
        else if (state.player->position.x >= 11 && state.player->position.x < 12){
            Util::DrawText(program, fontTextureID2, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(8, -2.5,0.0f));
            //std::cout<<"here3";
        }
        
        else{
            Util::DrawText(program, fontTextureID2, "YOU LOST" , 2.0f, -1.0f, glm:: vec3(10, -2.5,0.0f));
        }
    
        state.paused = true;
    }
    
    
}
