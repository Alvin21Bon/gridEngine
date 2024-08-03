#version 330 core

// instanced arrays
layout (location = 0) in vec3 aPixelColor;
layout (location = 1) in uint isPixelVisible;

// square unit model data
layout (location = 2) in vec2 vertexPosition;

// used for figuring out where each instance should go in the grid
uniform uvec2 gridUnitCnt;

out vec3 pixelColor;

void main()
{
	// determine the row and column position of this instance
	uint columnPos = uint(gl_InstanceID) / gridUnitCnt.y;
	uint rowPos = uint(gl_InstanceID) % gridUnitCnt.y;

	// figure out the NDC to move the vertices of this instance to for its column/row placement
	vec2 viewportBottomLeftCoords = vec2(-1, -1);

	float gridUnitWidthNDC = 2.0 / gridUnitCnt.x;
	float gridUnitHeightNDC = 2.0 / gridUnitCnt.y;
	vec2 gridOffsetAmount = vec2(columnPos * gridUnitWidthNDC, rowPos * gridUnitHeightNDC);

	vec2 translatedPosition = viewportBottomLeftCoords + gridOffsetAmount + vertexPosition;

	// chuck this vertex out of range if this instance is supposed to not be visible
	bool boolIsPixelVisible = bool(isPixelVisible);
	translatedPosition = translatedPosition + (int(!boolIsPixelVisible) * 100);

	gl_Position = vec4(translatedPosition, 0.0, 1.0);

	pixelColor = aPixelColor;
}
