"#version 330 core\n"

"// instanced arrays\n"
"layout (location = 0) in vec3 aPixelColor;\n"
"layout (location = 1) in uint isPixelVisible;\n"

"// square unit model data\n"
"layout (location = 2) in vec2 vertexPosition;\n"

"// used for figuring out where each instance should go in the grid\n"
"uniform uvec2 gridUnitCnt;\n"

"out vec3 pixelColor;\n"

"void main()\n"
"{\n"
"	// determine the row and column position of this instance\n"
"	uint columnPos = uint(gl_InstanceID) / gridUnitCnt.y;\n"
"	uint rowPos = uint(gl_InstanceID) % gridUnitCnt.y;\n"

"	// figure out the NDC to move the vertices of this instance to for its column/row placement\n"
"	vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"

"	float gridUnitWidthNDC = 2.0 / gridUnitCnt.x;\n"
"	float gridUnitHeightNDC = 2.0 / gridUnitCnt.y;\n"
"	vec2 gridOffsetAmount = vec2(columnPos * gridUnitWidthNDC, rowPos * gridUnitHeightNDC);\n"

"	vec2 translatedPosition = viewportBottomLeftCoords + gridOffsetAmount + vertexPosition;\n"

"	// chuck this vertex out of range if this instance is supposed to not be visible\n"
"	bool boolIsPixelVisible = bool(isPixelVisible);\n"
"	translatedPosition = translatedPosition + (int(!boolIsPixelVisible) * 100);\n"

"	gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"

"	pixelColor = aPixelColor;\n"
"}\0"
