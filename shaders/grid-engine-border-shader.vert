#version 330 core

// re-using square unit model data for the border
layout (location = 2) in vec2 vertexPosition;

uniform vec3 borderColor;
uniform vec2 aCanvasBottomLeftCoordsInNDC; // holds the location of the canvas within the perspective of the border viewport

out vec3 pixelColor;
out vec2 canvasBottomLeftCoordsInNDC;
out vec2 canvasTopRightCoordsInNDC;
out vec2 pixelCoordsInNDC; // used in fragment shader for discarding fragments within the coordinates of the canvas region

void main()
{
	// scale up to fill the entire viewport
	vec2 translatedPosition = vertexPosition * 1000000;

	vec2 viewportBottomLeftCoords = vec2(-1, -1);
	translatedPosition = translatedPosition + viewportBottomLeftCoords;

	gl_Position = vec4(translatedPosition, 0.0, 1.0);
	pixelCoordsInNDC = gl_Position.xy;

	// the canvas top right coords are just the opposite of the bottom left coords
	canvasBottomLeftCoordsInNDC = aCanvasBottomLeftCoordsInNDC;
	canvasTopRightCoordsInNDC = canvasBottomLeftCoordsInNDC * -1;

	pixelColor = borderColor;
}
