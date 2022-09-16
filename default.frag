#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;
in vec3 normal;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;

vec3 sunDirection = normalize(vec3(-0.5, -1, -1));

void main()
{
	FragColor = texture(tex0, texCoord) * (dot(-sunDirection, normal) / 2. + .5);
}