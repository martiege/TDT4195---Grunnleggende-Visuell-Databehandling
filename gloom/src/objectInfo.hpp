#pragma once
// #include "mesh.hpp"
// #include "sceneGraph.hpp"
// #include <string>

// enum Model 
// {
//     ml_terrain, 
//     ml_body, 
//     ml_mainRotor, 
//     ml_tailRotor, 
//     ml_door, 
//     ml_cursedBody, 
//     ml_cursedEyes
// };


// // NOTE TO SELF
// // RATHER THAN DOING THIS SHIT YOU COULD'VE CHANGED THE SCENENODE CLASS
// // YAY I DID IT WOW THIS IS MUCH BETTER!
// class ObjectInfo
// {
// public:
//     ObjectInfo(Model name, Mesh* mesh, unsigned int vaoID) 
//         : name(name), mesh(mesh), sceneNodes(), vaoID(vaoID) {}
    
//     Model name;
//     Mesh* mesh; 
//     std::vector<SceneNode*> sceneNodes;
//     unsigned int vaoID;
// };