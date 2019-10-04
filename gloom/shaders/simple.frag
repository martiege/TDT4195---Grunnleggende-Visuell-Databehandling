#version 450 core

in vec4 gl_FragCoord;
out vec4 color;

in layout(location=1) vec4 vertexColour;
in layout(location=2) vec3 normalVector;

vec3 rainbow(float x)
{
	/* 
    From: 
    https://github.com/wsmind/js-pride/blob/master/shaders/rainbow.glsl

		Target colors
		=============
		
		L  x   color
		0  0.0 vec4(1.0, 0.0, 0.0, 1.0);
		1  0.2 vec4(1.0, 0.5, 0.0, 1.0);
		2  0.4 vec4(1.0, 1.0, 0.0, 1.0);
		3  0.6 vec4(0.0, 0.5, 0.0, 1.0);
		4  0.8 vec4(0.0, 0.0, 1.0, 1.0);
		5  1.0 vec4(0.5, 0.0, 0.5, 1.0);
	*/
	
	float level = floor(x * 6.0);
	float r = float(level <= 2.0) + float(level > 4.0) * 0.5;
	float g = max(1.0 - abs(level - 2.0) * 0.5, 0.0);
	float b = (1.0 - (level - 4.0) * 0.5) * float(level >= 4.0);
	return vec3(r, g, b);
}

vec4 checkerboard(vec4 coor) 
{
    float c = (int(coor.x  / 10) % 2) == (int(coor.y / 10) % 2) ? 1.0f : 0.0f;
    return vec4(
        c, c, c, 1.0f
    );
}

void main()
{
    vec3 rb = rainbow(gl_FragCoord.x / 1024);

	vec3 lightDirection = normalize(vec3(0.8, -0.5, 0.6));
	float lightConstant = - dot(normalVector, lightDirection);
	// new variable, to keep the original alpha (could only save original alpha, but i'm lazy)
	vec4 lightedColour = vertexColour * max(0, lightConstant);

    color = 
		// vec4(normalVector.x, normalVector.y, normalVector.z, 1.0f);
		vec4(lightedColour.r, lightedColour.g, lightedColour.b, vertexColour.a);
        // vec4(rb.x, rb.y, rb.z, 1.0f);
        // vec4(1.0f, 1.0f, 1.0f, 1.0f);
        // checkerboard(gl_FragCoord);
}
