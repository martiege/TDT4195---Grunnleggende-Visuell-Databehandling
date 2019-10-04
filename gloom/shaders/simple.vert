#version 450 core

in vec3 position;
in layout(location=1) vec4 vertexColour;
in layout(location=2) vec3 normalVector;

uniform layout(location=3) mat4x4 modelMatrix;
uniform layout(location=4) mat4x4 matrixProjection;

out layout(location=1) vec4 vertexColourOut;
out layout(location=2) vec3 normalVectorOut;

void main()
{
    vertexColourOut = vertexColour;
    // vec4 transformedNormalVector = matrix * vec4(
    //     normalVector.x, 
    //     normalVector.y, 
    //     normalVector.z, 
    //     1.0
    // );
    normalVectorOut =
        // normalVector;
        normalize(mat3(modelMatrix) * normalVector);
    //     vec3(
    //         transformedNormalVector.x, 
    //         transformedNormalVector.y, 
    //         transformedNormalVector.z
    // );

    gl_Position = matrixProjection * 
        vec4(position, 1.0f);
}
