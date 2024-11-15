"#version 330 core\n"

"// instanced arrays\n"
"layout (location = 0) in vec3 aPixelColor;\n"
"layout (location = 1) in uint isPixelVisible;\n"

"// unit square model data\n"
"layout (location = 2) in vec2 unitSquareVertexPosition;\n"

"// used for figuring out where each instance should go in the grid\n"
"uniform uvec2 gridUnitCnt;\n"

"out vec3 pixelColor;\n"

"void main()\n"
"{\n"
" 	//scale unit square vertices to size of canvas grid unit square for actual vertex pos\n"
" 	vec2 canvasGridUnitSquare = vec2(2.0 / gridUnitCnt.x, 2.0 / gridUnitCnt.y);\n"
" 	vec2 vertexPosition = unitSquareVertexPosition * canvasGridUnitSquare;\n"

"	// determine the row and column coords of this instance\n"
" 	uvec2 gridInstanceCoords = uvec2(uint(gl_InstanceID) / gridUnitCnt.y, uint(gl_InstanceID) % gridUnitCnt.y);\n"

"	// figure out the NDC to move the vertices of this instance to for its column/row placement\n"
"	vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"
"	vec2 gridOffsetAmount = gridInstanceCoords * canvasGridUnitSquare;\n"

"	vec2 translatedPosition = viewportBottomLeftCoords + gridOffsetAmount + vertexPosition;\n"

"	// chuck this vertex out of range if this instance is supposed to not be visible\n"
"	translatedPosition = translatedPosition + (int(!bool(isPixelVisible)) * 100);\n"

"	gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"

"	pixelColor = aPixelColor;\n"
"}\0"
