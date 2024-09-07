#version 330 core

// re-using square unit model data for the border
layout (location = 2) in vec2 vertexPosition;

uniform vec3 borderColor;

out vec3 pixelColor;

void main()
{
	// scale up to fill the entire viewport
	vec2 translatedPosition = vertexPosition * 100;

	vec2 viewportBottomLeftCoords = vec2(-1, -1);
	translatedPosition = translatedPosition + viewportBottomLeftCoords;

	gl_Position = vec4(translatedPosition, 0.0, 1.0);

	pixelColor = borderColor;
}
