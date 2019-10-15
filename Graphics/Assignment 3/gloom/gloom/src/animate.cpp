#include "animate.hpp"

#include "toolbox.hpp"

#include <iostream>

// rotate negative around x
const glm::vec3 TAIL_ROTOR_ROTATION_SPEED = glm::vec3(-15, 0, 0);
// rotate around y
const glm::vec3 MAIN_ROTOR_ROTATION_SPEED = glm::vec3(0, 10, 0);
const float MOVE_SPEED = 5;
// rotate around z
glm::vec3 DOOR_ROTATION_SPEED = glm::vec3(0, 0, 0);
bool doorOpen = false;

void animateTailRotor(SceneNode* tailRotor, float deltaTime)
{
    tailRotor->rotation +=
        deltaTime * TAIL_ROTOR_ROTATION_SPEED;
}

void animateMainRotor(SceneNode* mainRotor, float deltaTime)
{
    mainRotor->rotation +=
        deltaTime * MAIN_ROTOR_ROTATION_SPEED;
}

void animateDoor(SceneNode* door, float deltaTime)
{
    // if outside range, change direction only once
    bool changedDirection = ((door->rotation.z > glm::pi<double>() / 2.0)
    && (DOOR_ROTATION_SPEED.z > 0))
    || ((door->rotation.z < 0)
    && (DOOR_ROTATION_SPEED.z < 0));

    if (changedDirection)
    {
        DOOR_ROTATION_SPEED *= -1;
        if (DOOR_ROTATION_SPEED.z > 0)
        {
            DOOR_ROTATION_SPEED.z = 0;
        }
    }

    door->rotation +=
        deltaTime * DOOR_ROTATION_SPEED;
}

void startDoorAnimation()
{
    DOOR_ROTATION_SPEED = glm::vec3(0, 0, 1);
}

void animateBody(SceneNode* body, float totalTime)
{
    Heading heading = simpleHeadingAnimation(totalTime);
    body->position = glm::vec3(heading.x, body->position.y, heading.z);
    // something seems off when setting this...
    // I've tried all combinations, and this seems to be 
    // the best
    body->rotation = glm::vec3(
        heading.pitch,
        heading.yaw,
        heading.roll
    );
}

void animateCursed(SceneNode* cursed, float totalTime, unsigned int k)
{
    Heading heading = cursedHeading(totalTime, k);
    cursed->position = glm::vec3(heading.x, cursed->position.y, heading.z);
    int round = static_cast<int>(totalTime / 10.0);
    if (round % 3 == 0)
    {
        cursed->rotation += glm::vec3(0.1, 0, 0);
    }
    else if (round % 3 == 1)
    {
        cursed->rotation += glm::vec3(0, 0.1, 0);
    }
    else 
    {
        cursed->rotation += glm::vec3(0, 0, 0.1);
    }
    
}

bool move(SceneNode* root, float deltaTime, float newYpos)
{
    glm::vec3 delta(
        0, 
        MOVE_SPEED * sgn(newYpos - root->position.y) * deltaTime, 
        0
    );

    root->position += delta;
    return (newYpos - root->position.y) < 1;
}

bool multimove(SceneNode* root, float deltaTime)
{
    switch (root->name)
    {
    case ml_body:
        return move(root, deltaTime, -40);
    case ml_cursedBody: 
        return move(root, deltaTime, 20);
    default:
        break;
    }

    for (SceneNode* child : root->children)
        if (!multimove(child, deltaTime))
            return false;
    return true; 
}

void animate(SceneNode* root, float totalTime, float deltaTime, unsigned int k)
{
    switch (root->name)
    {
    case ml_body:
        animateBody(root, totalTime);
        break;
    case ml_mainRotor:
        animateMainRotor(root, deltaTime);
        break;
    case ml_tailRotor:
        animateTailRotor(root, deltaTime);
        break;
    case ml_door:
        animateDoor(root, deltaTime);
        break;
    case ml_cursedBody:
        animateCursed(root, totalTime, k);
        break;
    default:
        break;
    }

    for (SceneNode* child : root->children)
        animate(child, totalTime, deltaTime, k);
}