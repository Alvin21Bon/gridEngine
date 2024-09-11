#version 330 core

in vec3 pixelColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(pixelColor, 1.0);
}
