#include "triangle.hpp"

// Local headers
#include "gloom/gloom.hpp"
#include "program.hpp"

// System headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard headers
#include <cstdlib>

unsigned int createTriangleVertexArrayObject(
    std::vector<float> vertices, 
    std::vector<unsigned int> indices
) 
{
    unsigned int vaoID = 0; 
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    unsigned int vertexBufferID = 0; 
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    unsigned int indexBuffer = 0; 
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        indices.size() * sizeof(int), 
        indices.data(), 
        GL_STATIC_DRAW
    );

    return vaoID;
}

unsigned int createColouredTriangleVertexArrayObject(
    std::vector<float> vertices, 
    std::vector<float> colours, 
    std::vector<unsigned int> indices
)
{
    // 4 * colours.size() == 3 * vertices.size()?
    unsigned int vaoID = 0; 
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    unsigned int vertexBufferID = 0; 
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    unsigned int colourBufferID = 0; 
    glGenBuffers(1, &colourBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);

    glBufferData(
        GL_ARRAY_BUFFER,
        colours.size() * sizeof(float),
        colours.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    unsigned int indexBuffer = 0; 
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        indices.size() * sizeof(unsigned int), 
        indices.data(), 
        GL_STATIC_DRAW
    );

    return vaoID;
}

void drawTriangleVertexArrayObject(unsigned int vaoID, unsigned int indexCount)
{
    glBindVertexArray(vaoID);
    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );
}
