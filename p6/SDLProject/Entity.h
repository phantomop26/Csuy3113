#pragma once
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
#include "Map.h"

enum EntityType {PLAYER, PLATFORM, ENEMY, STAR};
enum AIType {JUMPER, DISAPPEARING, MOVING_LEFT_RIGHT};
enum AIState {IDLE, WALKING, ATTACKING, JUMPING};



class Entity {
public:
    bool isVisible = true;
    float disappearTime = 1.0f;
    float appearTime = 1.0f;
    float timeSinceLastChange = 0.0f;
    bool isDisappearing = false;
    bool defeated = false;
    int lives =3;
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    float moveSpeed = 1.0f;
    glm::vec3 startPos;
    glm::vec3 endPos;
    bool movingRight = true;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    float width =1;
    float height=1;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;
    
    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    bool jump = false;
    float jumpPower =0;
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool pit_left_detected = false;
    bool pit_right_detected = false;
    bool collidedWithCoin  = false;
    
    int initPosX =0;
    
    Entity();
    bool CheckCollision(Entity *other);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map, Entity *stars, int starCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    void AI(Entity* player, float deltaTime);
    void Jumper(Entity* Player);
    void DisappearingEntity(float deltaTime);
    void MovingLeftRight(float deltaTime);
};
