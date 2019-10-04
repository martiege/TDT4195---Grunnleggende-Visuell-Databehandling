#pragma once

#include <vector>


unsigned int createTriangleVertexArrayObject(
    std::vector<float> vertices, 
    std::vector<unsigned int> indices
);

unsigned int createColouredTriangleVertexArrayObject(
    std::vector<float> vertices, 
    std::vector<float> colours, 
    std::vector<unsigned int> indices
);

void drawTriangleVertexArrayObject(unsigned int vaoID, unsigned int indexCount);

