#include "scene.hpp"

#include "vao.hpp"
#include "gloom/gloom.hpp"
// glm
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

SceneNode* setUpTerrain(Mesh* terrainMesh)
{
    // Terrain
    SceneNode* terrainSceneNode = createSceneNode();
    terrainSceneNode->vertexArrayObjectID = createVAO(terrainMesh);
    terrainSceneNode->VAOIndexCount = terrainMesh->indices.size();
    terrainSceneNode->name = ml_terrain;
    terrainSceneNode->mesh = terrainMesh;

    return terrainSceneNode; 
}

SceneNode* setUpHelicopter(Helicopter* helicopter)
{
    // Helicopter

    // Body
    SceneNode* helicopterBodySceneNode = createSceneNode(); 
    helicopterBodySceneNode->vertexArrayObjectID 
        = createVAO(&(helicopter->body));
    helicopterBodySceneNode->VAOIndexCount 
        = (helicopter->body).indices.size();
    helicopterBodySceneNode->name = ml_body;
    helicopterBodySceneNode->mesh = &helicopter->body;

    // Door
    SceneNode* helicopterDoorSceneNode = createSceneNode();
    helicopterDoorSceneNode->vertexArrayObjectID 
        = createVAO(&(helicopter->door));
    helicopterDoorSceneNode->VAOIndexCount 
        = (helicopter->door).indices.size();
    helicopterDoorSceneNode->name = ml_door;
    helicopterDoorSceneNode->mesh = &(helicopter->door);

    helicopterBodySceneNode->children.push_back(
        helicopterDoorSceneNode
    );

    // Tail rotor
    SceneNode* helicopterTailRotorSceneNode = createSceneNode();
    helicopterTailRotorSceneNode->vertexArrayObjectID 
        = createVAO(&(helicopter->tailRotor));
    helicopterTailRotorSceneNode->VAOIndexCount 
        = (helicopter->tailRotor).indices.size();
    helicopterTailRotorSceneNode->name = ml_tailRotor;
    helicopterTailRotorSceneNode->mesh = &(helicopter->tailRotor);helicopterTailRotorSceneNode->referencePoint 
        = glm::vec3(0.35, 2.3, 10.4);

    helicopterBodySceneNode->children.push_back(
        helicopterTailRotorSceneNode
    );

    // Main rotor
    SceneNode* helicopterMainRotorSceneNode = createSceneNode();
    helicopterMainRotorSceneNode->vertexArrayObjectID 
        = createVAO(&(helicopter->mainRotor));
    helicopterMainRotorSceneNode->VAOIndexCount 
        = (helicopter->mainRotor).indices.size();
    helicopterMainRotorSceneNode->name = ml_mainRotor;
    helicopterMainRotorSceneNode->mesh = &(helicopter->mainRotor);

    helicopterBodySceneNode->children.push_back(
        helicopterMainRotorSceneNode
    );

    return helicopterBodySceneNode;
}

SceneNode* setUpCursed(CursedMesh* cursed)
{
    // Body
    SceneNode* cursedBody = createSceneNode(); 
    cursedBody->vertexArrayObjectID 
        = createVAO(&(cursed->body));
    cursedBody->VAOIndexCount 
        = cursed->body.indices.size();
    cursedBody->name = ml_cursedBody;
    cursedBody->mesh = &(cursed->body);

    cursedBody->position = glm::vec3(0, -40, 0);

    // Eyes
    SceneNode* cursedEyes = createSceneNode(); 
    cursedEyes->vertexArrayObjectID 
        = createVAO(&(cursed->eyes));
    cursedEyes->VAOIndexCount 
        = cursed->body.indices.size(); 
    cursedEyes->name = ml_cursedEyes; 
    cursedEyes->mesh = &(cursed->eyes);

    cursedBody->children.push_back(cursedEyes);

    return cursedBody;
}

SceneNode* setUpScene(Mesh* terrainMesh, Helicopter* helicopter, CursedMesh* cursed)
{
    SceneNode* root = createSceneNode(); 

    root->children.push_back(
        setUpTerrain(terrainMesh)
    );

    root->children.push_back(
        setUpHelicopter(helicopter)
    );

    root->children.push_back(
        setUpCursed(cursed)
    );  

    return root;
}

// SceneNode* setUpScene(std::vector<ObjectInfo>& data)
// {
//     SceneNode* root = createSceneNode();
//     SceneNode* terrainSceneNode = nullptr; 
//     SceneNode* helicopterBodySceneNode = nullptr; 
//     SceneNode* cursedBody = nullptr;
//     unsigned int initialNumScenes;

//     SceneNode* sceneNode;

//     for (unsigned int i = 0; i < data.size(); ++i) 
//     {
//         initialNumScenes = data.at(i).sceneNodes.size();

//         sceneNode = createSceneNode();
//         sceneNode->vertexArrayObjectID = (data.at(i)).vaoID;
//         sceneNode->VAOIndexCount = ((data.at(i)).mesh->indices).size();
//         data.at(i).sceneNodes.push_back(sceneNode);

//         switch (data.at(i).name)
//         {
//         case ml_terrain:
//             terrainSceneNode = data.at(i).sceneNodes.at(initialNumScenes);
//             addChild(root, data.at(i).sceneNodes.at(initialNumScenes));
//             break;
//         case ml_body:
//             helicopterBodySceneNode = data.at(i).sceneNodes.at(initialNumScenes);
//             if (terrainSceneNode != nullptr)
//             {
//                 addChild(
//                     terrainSceneNode, 
//                     data.at(i).sceneNodes.at(initialNumScenes)
//                 );
//             }
//             break;
//         case ml_cursedBody:
//             cursedBody = data.at(i).sceneNodes.at(initialNumScenes);
//             if (terrainSceneNode != nullptr)
//             {
//                 addChild(
//                     terrainSceneNode, 
//                     data.at(i).sceneNodes.at(initialNumScenes)
//                 );
//             }
//             break;
//         case ml_cursedEyes:
//             if (cursedBody != nullptr)
//             {
//                 addChild(
//                     terrainSceneNode, 
//                     data.at(i).sceneNodes.at(initialNumScenes)
//                 );
//             }
//             break;
//         case ml_tailRotor:
//             sceneNode->referencePoint = glm::vec3(0.35, 2.3, 10.4);
//         default:
//             if (helicopterBodySceneNode != nullptr) 
//             {
//                 addChild(
//                     helicopterBodySceneNode, 
//                     data.at(i).sceneNodes.at(initialNumScenes)
//                 );
//             }
//             break;
//         }
//     }


//     return root;
// }

void drawSceneNode(SceneNode* root, glm::mat4 viewProjectionMatrix) 
{
    if (root->vertexArrayObjectID >= 0)
    {
        // set uniform for all but root 
        glUniformMatrix4fv(4, 1, GL_FALSE, 
            glm::value_ptr(
                viewProjectionMatrix 
                * root->currentTransformationMatrix 
            )
        );

        glUniformMatrix4fv(3, 1, GL_FALSE, 
            glm::value_ptr(
                root->currentTransformationMatrix 
            )
        );

        glBindVertexArray(root->vertexArrayObjectID);
        glDrawElements(
            GL_TRIANGLES,
            root->VAOIndexCount,
            GL_UNSIGNED_INT,
            0
        );
    }
    
    for (SceneNode* child : root->children)
        drawSceneNode(child, viewProjectionMatrix);
}

void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar)
{
    // transformation computations
    glm::mat4 combinedTransformation = 
        transformationThusFar 
        * glm::translate(node->position)
        * glm::translate(node->referencePoint) 
        * glm::rotate(node->rotation.x, glm::vec3(1,0,0))
        * glm::rotate(node->rotation.y, glm::vec3(0,1,0))
        * glm::rotate(node->rotation.z, glm::vec3(0,0,1))
        * glm::translate(- node->referencePoint);
 

    // store matrix
    node->currentTransformationMatrix = combinedTransformation;

    for (SceneNode* child : node->children)
        updateSceneNode(child, combinedTransformation);
}
