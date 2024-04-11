#version 420 core

out vec4 FragColor;

in vec3 color;
in vec2 textureCoordinates;

uniform sampler2D diffuse;

void main() 
{
	FragColor = texture(diffuse, textureCoordinates) * vec4(color, 1.0);
}