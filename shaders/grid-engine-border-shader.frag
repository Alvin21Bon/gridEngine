"#version 330 core\n"

"in vec3 pixelColor;\n"
"in vec2 canvasBottomLeftCoordsInNDC;\n"
"in vec2 canvasTopRightCoordsInNDC;\n"
"in vec2 pixelCoordsInNDC;\n"

"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"	// disard fragment if in the region of the canvas rendering\n"
"	if (all(greaterThan(pixelCoordsInNDC, canvasBottomLeftCoordsInNDC)) && all(lessThan(pixelCoordsInNDC, canvasTopRightCoordsInNDC))) discard;\n"

"	FragColor = vec4(pixelColor, 1.0);\n"
"}\0"
