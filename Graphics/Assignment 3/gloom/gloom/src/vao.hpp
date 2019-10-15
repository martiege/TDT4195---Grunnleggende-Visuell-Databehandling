#pragma once

// Local headers
#include "mesh.hpp"
// #include "objectInfo.hpp"

// creates a single VAO given a pointer to a mesh
unsigned int createVAO(Mesh* mesh);

// sets VAOs for a vector of pointers to meshes
// doesn't really matter that it is a reference, 
// as the values are only pointers anyways
// void createAllVAOs(std::vector<ObjectInfo>& data);

