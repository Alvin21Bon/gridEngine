#version 330 core

in vec3 pixelColor;
in vec2 canvasBottomLeftCoordsInNDC;
in vec2 canvasTopRightCoordsInNDC;
in vec2 pixelCoordsInNDC;

out vec4 FragColor;

void main()
{
	// disard fragment if in the region of the canvas rendering
	if (all(greaterThan(pixelCoordsInNDC, canvasBottomLeftCoordsInNDC)) && all(lessThan(pixelCoordsInNDC, canvasTopRightCoordsInNDC))) discard;
	
	FragColor = vec4(pixelColor, 1.0);
}
