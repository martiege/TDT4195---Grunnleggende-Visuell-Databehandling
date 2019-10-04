#pragma once

#include "sceneGraph.hpp"

// glm
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

void animateTailRotor(SceneNode* tailRotor, float deltaTime);

void animateMainRotor(SceneNode* mainRotor, float deltaTime);

void animateDoor(SceneNode* door, float deltaTime);

void startDoorAnimation();

void animateBody(SceneNode* body, float totalTime);

bool move(SceneNode* root, float deltaTime, float newYpos);

bool multimove(SceneNode* root, float deltaTime);

void animate(SceneNode* root, float totalTime, float deltaTime, unsigned int k);