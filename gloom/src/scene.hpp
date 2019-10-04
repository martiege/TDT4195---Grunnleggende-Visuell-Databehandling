#pragma once

#include "sceneGraph.hpp"
#include "OBJLoader.hpp"
#include "mesh.hpp"
// #include "objectInfo.hpp"

#include <vector>

SceneNode* setUpScene(Mesh* terrainMesh, Helicopter* helicopter, CursedMesh* cursed);
// SceneNode* setUpScene(std::vector<ObjectInfo>& data);

void drawSceneNode(SceneNode* root, glm::mat4 viewProjectionMatrix);

void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar);
