// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
// #include "triangle.hpp"
// #include "linspace.hpp"
// #include "utilities.hpp"
#include "OBJLoader.hpp"
#include "vao.hpp"
#include "scene.hpp"
// #include "objectInfo.hpp"
#include "toolbox.hpp"
#include "animate.hpp"

// glm
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Standard headers
#include <vector>
#include <algorithm>
#include <map>
// std::for_each(vec.begin(), vec.end(), func);

// speed variables for easier camera control
const float ROT_SPEED = 0.01;
const float COORD_SPEED = 1;

float rotX = 0;
float rotY = 0;
float rotZ = 0;
float cordX = 0;
float cordY = 0;
float cordZ = -3.0f;

bool moving = false;

void runProgram(GLFWwindow* window)
{
    Gloom::Shader shader;
    shader.attach("../gloom/shaders/simple.frag");
    shader.attach("../gloom/shaders/simple.vert");
    shader.link();

    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up your scene here (create Vertex Array Objects, etc.)

    // OwO
    CursedMesh OwO = 
        loadCursedModel("../gloom/resources/OwO.obj");

    // Terrain
    Mesh terrainMesh = 
        loadTerrainMesh("../gloom/resources/lunarsurface.obj");
    // Helicopter
    Helicopter helicopterMesh = 
        loadHelicopterModel("../gloom/resources/helicopter.obj");
    
    // convert to vector of ObjectInfo, to simplify
    // std::vector<ObjectInfo> data
    // {
    //     ObjectInfo(ml_terrain, &(terrainMesh), 0),
    //     ObjectInfo(ml_body, &(helicopterMesh.body), 0),
    //     ObjectInfo(ml_mainRotor, &(helicopterMesh.mainRotor), 0),
    //     ObjectInfo(ml_tailRotor, &(helicopterMesh.tailRotor), 0),
    //     ObjectInfo(ml_door, &(helicopterMesh.door), 0),
    //     ObjectInfo(ml_cursedBody, &(OwO.body), 0),
    //     ObjectInfo(ml_cursedEyes, &(OwO.eyes), 0),
    // };

    // fill vaoID data
    // createAllVAOs(data);
    std::vector<SceneNode*> roots;
    for (unsigned int i = 0; i < 5; ++i)
        roots.push_back(setUpScene(&terrainMesh, &helicopterMesh, &OwO));

    // Camera
    glm::mat4x4 identityMatrix = glm::mat4(1.0f);
    glm::mat4x4 perspectiveMatrix = glm::perspective(
        glm::radians(45.0f), 
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 
        1.0f, 1000.0f
    );

    float deltaTime = 0.0; 
    float totalTime = 0.0;
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // printf("Current time: %f\n", totalTime);
        deltaTime = getTimeDeltaSeconds();
        totalTime += deltaTime;

        // Clear colour and depth buffers
        glClear(
            GL_COLOR_BUFFER_BIT 
            | GL_DEPTH_BUFFER_BIT
        );
        
        shader.activate();

        if (moving)
        {
            rotX += 0.02 * (randomUniformFloat() - 0.5);
            rotY += 0.02 * (randomUniformFloat() - 0.5);
            rotZ += 0.02 * (randomUniformFloat() - 0.5);
        }

        glm::mat4 translationMatrix = glm::translate(glm::vec3(-cordX, -cordY, cordZ));

        glm::mat4 xRotateMatrix = glm::rotate(rotX, glm::vec3(1,0,0));
        glm::mat4 yRotateMatrix = glm::rotate(rotY, glm::vec3(0,1,0));
        glm::mat4 zRotateMatrix = glm::rotate(rotZ, glm::vec3(0,0,1));
        

        glm::mat4 finalTransformationMatrix = 
            perspectiveMatrix * translationMatrix * zRotateMatrix * yRotateMatrix * xRotateMatrix * identityMatrix;

        glUniformMatrix4fv(4, 1, GL_FALSE, 
            glm::value_ptr(finalTransformationMatrix)
        );

        for (unsigned int i = 0; i < roots.size(); ++i)
        {
            animate(roots.at(i), totalTime + i * 0.75, deltaTime, roots.size());
            if (moving)
            {
                if (multimove(roots.at(i), deltaTime))
                    moving = false;
            }
            // update scene
            updateSceneNode(roots.at(i), glm::mat4(1.0));

            // Draw your scene here
            drawSceneNode(roots.at(i), finalTransformationMatrix);
        }
        
        // for (unsigned int i = 0; i < data.size(); ++i)
        // {
        //     glBindVertexArray(data.at(i).vaoID);
        //     glDrawElements(
        //         GL_TRIANGLES,
        //         (data.at(i).mesh)->vertexCount(),
        //         GL_UNSIGNED_INT,
        //         0
        //     );
        // }
        

        shader.deactivate();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }

    shader.destroy();
}

void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // translations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        // forwards
        cordZ += COORD_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // backwards
        cordZ -= COORD_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        // up
        cordY += COORD_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        // down
        cordY -= COORD_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // right
        cordX += COORD_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // left
        cordX -= COORD_SPEED;
    }

    // rotations
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        // forwards
        rotZ += ROT_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        // backwards
        rotZ -= ROT_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        // up
        rotY += ROT_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        // down
        rotY -= ROT_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        // right
        rotX += ROT_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        // left
        rotX -= ROT_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        startDoorAnimation();
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        std::cout << "cUrSeD....." << std::endl;
        cordZ = -250;
        cordY = 20;
        moving = true; 
    }
}

// // keeping this boi for sentimental (and debugging) purposes 
// void oldRunProgram(GLFWwindow* window)
// {
//     Gloom::Shader shader;
//     shader.attach("../gloom/shaders/simple.frag");
//     shader.attach("../gloom/shaders/simple.vert");
//     shader.link();

//     // Enable depth (Z) buffer (accept "closest" fragment)
//     glEnable(GL_DEPTH_TEST);
//     glDepthFunc(GL_LESS);

//     // Configure miscellaneous OpenGL settings
//     glEnable(GL_CULL_FACE);

//     // Set default colour after clearing the colour buffer
//     glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

//     // Transparency
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     // Set up your scene here (create Vertex Array Objects, etc.)
//     std::vector<float> triangleVertices =
//     {
//         -0.80, -0.27, 0.27, 
//          0.27, -0.27, 0.27, 
//         -0.27,  0.80, 0.27, 

//         -0.27, -0.27, 0.10, 
//          0.80, -0.27, 0.10, 
//          0.27,  0.80, 0.10, 

//         -0.27, -0.80, 0.05, 
//          0.80, -0.80, 0.05, 
//          0.27,  0.27, 0.05,
        
// //
// //         -0.9,  0.5, 0.0,
// //         -0.5,  0.5, 0.0,
// //         -0.9,  0.9, 0.0, 

// //         -0.9 + 0.5,  0.5, 0.0,
// //         -0.5 + 0.5,  0.5, 0.0,
// //         -0.9 + 0.5,  0.9, 0.0, 
        
// //         -0.9,  0.5 - 0.5, 0.0,
// //         -0.5,  0.5 - 0.5, 0.0,
// //         -0.9,  0.9 - 0.5, 0.0, 
        
// //         -0.9 + 0.5,  0.5 - 0.5, 0.0,
// //         -0.5 + 0.5,  0.5 - 0.5, 0.0,
// //         -0.9 + 0.5,  0.9 - 0.5, 0.0, 
        
// //         -0.9 + 1.0,  0.5, 0.0,
// //         -0.5 + 1.0,  0.5, 0.0,
// //         -0.9 + 1.0,  0.9, 0.0,   
// //

//     };
//     std::vector<float> triangleColours = 
//     {
//         1.0, 0.0, 0.0, 0.50, 
//         1.0, 0.0, 0.0, 0.50, 
//         1.0, 0.0, 0.0, 0.50, 

//         0.0, 1.0, 0.0, 0.50, 
//         0.0, 1.0, 0.0, 0.50, 
//         0.0, 1.0, 0.0, 0.50, 

//         0.0, 0.0, 1.0, 0.50, 
//         0.0, 0.0, 1.0, 0.50, 
//         0.0, 0.0, 1.0, 0.50, 


//         // 0.0, 0.0, 0.0, 1.0, 
//         // 0.0 + 0.5, 0.0, 0.0, 1.0, 
//         // 0.0 + 0.8, 0.0, 0.0, 1.0,

//         // 0.0, 0.0, 0.0, 1.0, 
//         // 0.0 + 0.5, 0.0 + 0.5, 0.0, 1.0, 
//         // 0.0 + 0.8, 0.0 + 0.8, 0.0, 1.0,

//         // 0.0, 0.0, 0.0, 1.0, 
//         // 0.0 + 0.5, 0.0, 0.0 + 0.5, 1.0, 
//         // 0.0 + 0.8, 0.0, 0.0 + 0.8, 1.0,

//         // 0.0, 0.0, 0.0, 1.0, 
//         // 0.0 + 0.5, 0.0 + 0.5, 0.0 + 0.5, 1.0, 
//         // 0.0 + 0.8, 0.0 + 0.8, 0.0 + 0.8, 1.0,

//         // 0.0, 1.0, 0.0, 1.0, 
//         // 0.0 + 0.5, 0.0, 0.0, 1.0, 
//         // 0.0 + 0.8, 0.0, 1.0, 1.0,  

//     };

//     // generate indices (so not reusing vertices)
//     std::vector<unsigned int> triangleIndices 
//     = {
//         // 2, 1, 0, 
//         // 5, 4, 3, 
//         // 8, 7, 6, 

//         6, 7, 8, 
//         3, 4, 5, 
//         0, 1, 2, 
//     }
//     ;
//     // for (unsigned int i = 0; i * 3 < triangleVertices.size(); ++i) 
//         // triangleIndices.push_back(i);

//     unsigned int triangleVaoID = createTriangleVertexArrayObject(
//         triangleVertices,
//         triangleIndices
//     ); 
//     unsigned int colouredTriangleVaoID = 
//     createColouredTriangleVertexArrayObject(
//         triangleVertices, triangleColours, triangleIndices
//     );

//     // float a = 1.0;
//     // float b = 0.0;
//     // float c = 0.0;
//     // float d = 0.0;
//     // float e = 1.0;
//     // float f = 0.0;
    
//     // float increment = 0.01; 
//     // unsigned int count = 0; 

//     glm::mat4x4 identityMatrix = glm::mat4(1.0f);
//     glm::mat4x4 perspectiveMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 1.0f, 100.0f);

//     // Rendering Loop
//     while (!glfwWindowShouldClose(window))
//     {
//         // Clear colour and depth buffers
//         glClear(
//             GL_COLOR_BUFFER_BIT 
//             | GL_DEPTH_BUFFER_BIT
//         );
        
//         shader.activate();

//         // if (count < 3)
//         //     count += rangedInput(a, increment, -0.5, 0.5);
//         // else if (count < 5)  
//         //     count += rangedInput(b, increment, -0.5, 0.5);
//         // else if (count < 7) 
//         //     count += rangedInput(c, increment, -0.5, 0.5);
//         // else if (count < 9) 
//         //     count += rangedInput(d, increment, -0.5, 0.5);
//         // else if (count < 11) 
//         //     count += rangedInput(e, increment, -0.5, 0.5);
//         // else if (count < 13) 
//         //     count += rangedInput(f, increment, -0.5, 0.5);

//         // glUniform4f(3, a, d, 0, 0);
//         // glUniform4f(4, b, e, 0, 0);
//         // glUniform4f(5, c, f, 0, 1); 
        

//         glm::mat4 translationMatrix = glm::translate(glm::vec3(-cordX, -cordY, cordZ));

//         glm::mat4 xRotateMatrix = glm::rotate(rotX, glm::vec3(1,0,0));
//         glm::mat4 yRotateMatrix = glm::rotate(rotY, glm::vec3(0,1,0));
//         glm::mat4 zRotateMatrix = glm::rotate(rotZ, glm::vec3(0,0,1));
        

//         glm::mat4 finalTransformationMatrix = 
//             perspectiveMatrix * zRotateMatrix * yRotateMatrix * xRotateMatrix * translationMatrix * identityMatrix;

//         glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(
//             finalTransformationMatrix
//             // identityMatrix
//             )
//         );

//         // Draw your scene here
//         // drawTriangleVertexArrayObject(triangleVaoID, triangleIndices.size());
//         drawTriangleVertexArrayObject(
//             colouredTriangleVaoID,                    
//             triangleIndices.size()
//         );

//         shader.deactivate();

//         // Handle other events
//         glfwPollEvents();
//         handleKeyboardInput(window);

//         // Flip buffers
//         glfwSwapBuffers(window);
//     }

//     shader.destroy();
// }