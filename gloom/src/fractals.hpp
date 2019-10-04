#pragma once

#include <vector>

unsigned int createColouredFractalVertexArrayObject(
    std::vector<float> vertices, 
    std::vector<float> colours, 
    std::vector<unsigned int> indices
);

void drawTriangleVertexArrayObject(
    unsigned int vaoID, 
    unsigned int indexCount
);

