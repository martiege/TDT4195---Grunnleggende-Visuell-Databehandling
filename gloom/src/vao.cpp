// Local headers
#include "vao.hpp"
#include "gloom/gloom.hpp"
#include "program.hpp"

// System headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard headers
#include <cstdlib>


unsigned int createVAO(Mesh* mesh)
{
    // initialize VAO
    unsigned int vaoID = 0; 
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // initialize VBO for vertices
    unsigned int vertexBufferID = 0; 
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh->vertices.size() * sizeof(float),
        mesh->vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // initialize VBO for colours
    unsigned int colourBufferID = 0; 
    glGenBuffers(1, &colourBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh->colours.size() * sizeof(float),
        mesh->colours.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);


    // initialize VBO for normals
    unsigned int normalBufferID = 0; 
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh->normals.size() * sizeof(float),
        mesh->normals.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // initialize indices
    unsigned int indexBuffer = 0; 
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        mesh->indices.size() * sizeof(unsigned int), 
        mesh->indices.data(), 
        GL_STATIC_DRAW
    );

    return vaoID;
}

// void createAllVAOs(std::vector<ObjectInfo>& data)
// {
//     // allocate vector to handle vao IDs
//     for (unsigned int i = 0; i < data.size(); ++i) 
//     {
//         data[i].vaoID = createVAO(data.at(i).mesh);
//     }
// }