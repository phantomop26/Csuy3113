#include "Entity.h"


#include "Scene.h"

Entity::Entity()
{
    speed = 0;
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other){
    if (other == this) return false;
    if (isActive == false || other->isActive == false ) return false;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) {
        return true;
    }
    return false;
}


void Entity::CheckCollisionsY(Entity *objects, int objectCount){
    for (int i = 0; i < objectCount; i++){
        Entity *object = &objects[i];
        
        if (CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if (this->entityType == PLAYER &&object -> entityType == STAR){
                    object->isActive = false;
                    this->collidedWithCoin = true;
                }
                
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (this->entityType == PLAYER &&object -> entityType == STAR){
                    object->isActive = false;
                    this->collidedWithCoin = true;
                }
            }
            
        }
    }
    
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount){
    for (int i = 0; i < objectCount; i++){
        Entity *object = &objects[i];
        if (CheckCollision(object)){
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if ((velocity.x >= 0 && this->entityType == PLAYER &&object -> entityType == ENEMY) || velocity.x > 0 ) { // we are moving to the right
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                if (this->entityType == PLAYER && object -> entityType == ENEMY){
                    this->defeated = true;
                    if ((this->lives) > 0 )this->lives -=1;
                    if ((this->lives) >= 1){
                        //std::cout << this->lives << '\n';
                        this->position = glm::vec3(1, -6,0);
                    }
                }
                else if (this->entityType == PLAYER && object -> entityType == STAR){
                    object->isActive = false;
                    this->collidedWithCoin = true;
                }
                
                
            }
            
            
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                if (this->entityType == PLAYER && object -> entityType == ENEMY){
                    this->defeated = true;
                    if ((this->lives) > 0 )this->lives -=1;
                    if ((this->lives) >= 1){
                        //std::cout << this->lives << '\n';
                        this->position = glm::vec3(1, -6,0);
                    }

                }
                else if(this->entityType == PLAYER && object -> entityType == STAR){
                    object->isActive = false;
                    this->collidedWithCoin = true;
                }
            }
            
        }
    }
    
}

void Entity::CheckCollisionsY(Map *map)
{
    if (velocity.y == 0) return;
    
    float penetration_x = 0;
    float penetration_y = 0;
    glm::vec3 probe = glm::vec3(position.x, position.y + (height / 2), position.z);
    
    if (velocity.y < 0) {
        probe.y = position.y - (height / 2);
    }
    
    map->IsSolid(probe, &penetration_x, &penetration_y);
    
    if (penetration_y == 0) {
        probe.x = position.x - (width / 2);
        map->IsSolid(probe, &penetration_x, &penetration_y);
    }
    
    if (penetration_y == 0) {
        probe.x = position.x + (width / 2);
        map->IsSolid(probe, &penetration_x, &penetration_y);
    }
    
    if (penetration_y > 0) {
        if (velocity.y > 0) {
            position.y = floor(position.y);
            collidedTop = true;
        }
        else if (velocity.y < 0) {
            position.y = ceil(position.y);
            collidedBottom = true;
        }
        velocity.y = 0;
    }
}



void Entity::CheckCollisionsX(Map *map)
{
    if (velocity.x == 0) return;
    
    float penetration_x = 0;
    float penetration_y = 0;
    glm::vec3 probe = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    if (velocity.x < 0) {
        probe.x = position.x - (width / 2);
    }
    
    map->IsSolid(probe, &penetration_x, &penetration_y);
    
    if (penetration_x == 0) {
        probe.y = position.y - (height / 2);
        map->IsSolid(probe, &penetration_x, &penetration_y);
    }
    
    if (penetration_x == 0) {
        probe.y = position.y + (height / 2);
        map->IsSolid(probe, &penetration_x, &penetration_y);
    }
    
    if (penetration_x > 0) {
        if (velocity.x > 0) {
            position.x = floor(position.x);
            collidedRight = true;
        }
        else if (velocity.x < 0) {
            position.x = ceil(position.x);
            collidedLeft = true;
        }
        velocity.x = 0;
    }
}

void Entity::Jumper(Entity* player){
    
    if (velocity.y == 0){
        velocity = glm::vec3(0,3.4,0);
    }
}

void Entity::MovingLeftRight(float deltaTime) {
    float moveDistance = moveSpeed * deltaTime;
    if (movingRight) {
        position.x += moveDistance;
        if (position.x >= startPos.x + 3.0f) {
            movingRight = false;
        }
    }
    else {
        position.x -= moveDistance;
        if (position.x <= endPos.x - 2.0f) {
            movingRight = true;
        }
    }
}

void Entity::AI(Entity* player, float deltaTime){
    switch (aiType) {
        case JUMPER:
            Jumper(player);
            break;
        case DISAPPEARING:
            DisappearingEntity(deltaTime);
            break;
        case MOVING_LEFT_RIGHT:
            MovingLeftRight(deltaTime);
            break;
    }
}

void Entity::DisappearingEntity(float deltaTime) {
    timeSinceLastChange += deltaTime;

    if (isDisappearing) {
        if (timeSinceLastChange >= disappearTime) {
            isVisible = true;
            timeSinceLastChange = 0.0f;
            isDisappearing = false;
        }
    } else {
        if (timeSinceLastChange >= appearTime) {
            isVisible = false;
            timeSinceLastChange = 0.0f;
            isDisappearing = true;
        }
    }
}
void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Map *map, Entity *stars, int starCount){
    if(isActive==false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entityType == ENEMY){
        AI(player, deltaTime);
    }
    
    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;
            
            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    
    if(jump){
        jump = false;
        velocity.y += jumpPower;
    }
    
    velocity.x = movement.x*speed;
    velocity += acceleration*deltaTime;
    
    if (entityType == PLAYER){
        velocity.y = movement.y * speed;
    }
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount);
    CheckCollisionsY(stars, starCount);
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount);
    CheckCollisionsX(stars, starCount);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index){
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
    
    program->SetModelMatrix(modelMatrix);
    
    if (isActive==false) return;
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    if (!isActive || !isVisible) return;
    program->SetModelMatrix(modelMatrix);

        if (animIndices != NULL) {
            DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
            return;
        }
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

