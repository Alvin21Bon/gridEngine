"#version 330 core\n"

"// re-using unit square model data for the border\n"
"layout (location = 2) in vec2 unitSquareVertexPosition;\n"

"uniform vec3 borderColor;\n"
"uniform vec2 aCanvasBottomLeftCoordsInNDC; // holds the location of the canvas within the perspective of the border viewport\n"

"out vec3 pixelColor;\n"
"out vec2 canvasBottomLeftCoordsInNDC;\n"
"out vec2 canvasTopRightCoordsInNDC;\n"
"out vec2 pixelCoordsInNDC; // used in fragment shader for discarding fragments within the coordinates of the canvas region\n"

"void main()\n"
"{\n"
"	// scale up to fill the entire viewport\n"
"	vec2 translatedPosition = unitSquareVertexPosition * 100;\n"

"	vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"
"	translatedPosition = translatedPosition + viewportBottomLeftCoords;\n"

"	gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"
"	pixelCoordsInNDC = gl_Position.xy;\n"

"	// the canvas top right coords are just the opposite of the bottom left coords\n"
"	canvasBottomLeftCoordsInNDC = aCanvasBottomLeftCoordsInNDC;\n"
"	canvasTopRightCoordsInNDC = canvasBottomLeftCoordsInNDC * -1;\n"

"	pixelColor = borderColor;\n"
"}\0"
