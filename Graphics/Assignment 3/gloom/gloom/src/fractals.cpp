// Local headers
#include "gloom/gloom.hpp"
#include "program.hpp"
#include "fractals.hpp"

// System headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard headers
#include <cstdlib>
#include <cmath>

void generateKochFractal(
    std::vector<float> &vertices, 
    std::vector<float> &colours
)
{
    // remove anything old
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(colours);
    
    // initialize
    float x1 = -1.0, y1 = -0.5; 
    float x2 =  1.0, y2 = -0.5;

    
}

void generateKochLine(
    std::vector<float> &vertices, 
    std::vector<float> &colours, 
    float x1, float y1, 
    float x2, float y2, 
    int times
)
{
    float x3, y3; 
    float x4, y4; 
    float x5, y5; 

    double theta = M_PI / 3.0;

    if (times > 0) 
    {
/*
        kochCurve(p1,p3,times-1);
		kochCurve(p3,p4,times-1);
		kochCurve(p4,p5,times-1);
		kochCurve(p5,p2,times-1);
        */
        x3 = (2.0 * x1 + x2) / 3.0;
        y3 = (2.0 * y1 + y2) / 3.0;

        x5 = (2.0 * x2 + x1) / 3.0;
        y5 = (2.0 * y2 + y1) / 3.0;

        x4 = x3 + (x5 - x3) * cos(theta) + (y5 - y3) * sin(theta);
        y4 = y3 + (x5 - x3) * sin(theta) + (y5 - y3) * cos(theta);

        generateKochLine(vertices, colours, x1, y1, x3, y3, times - 1);
        generateKochLine(vertices, colours, x3, y3, x4, y4, times - 1);
        generateKochLine(vertices, colours, x4, y4, x5, y5, times - 1);
        generateKochLine(vertices, colours, x5, y5, x2, y2, times - 1);
    }
    else 
    {
        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(0.0);

        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(0.0);

        // red
        colours.push_back(1.0);
        colours.push_back(0.0);
        colours.push_back(0.0);
        colours.push_back(1.0);

        // blue
        colours.push_back(0.0);
        colours.push_back(0.0);
        colours.push_back(1.0);
        colours.push_back(1.0);
    }
}

unsigned int createColouredFractalVertexArrayObject(
    std::vector<float> vertices, 
    std::vector<float> colours, 
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
        indices.size() * sizeof(int), 
        indices.data(), 
        GL_STATIC_DRAW
    );

    return vaoID;
}

// void drawTriangleVertexArrayObject(
//     unsigned int vaoID, 
//     unsigned int indexCount
// ) 
// {
//     glBindVertexArray(vaoID);
//         glDrawElements(
//             GL_LINES,
//             indexCount,
//             GL_UNSIGNED_INT,
//             0
//         );
// }
