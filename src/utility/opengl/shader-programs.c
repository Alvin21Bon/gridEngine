#include "utility/opengl/shader-programs.h"

#include "utility/opengl/glad.h"
#include "utility/math-util.h"
#include "utility/logging.h"
#include "canvas/coordinate-canvas.h"
#include <stdlib.h>

static char* loadFileIntoString(const char* fileName);
static void compileShader(unsigned int shaderID, int* success);
static ShaderProgram constructShaderProgramFromString(const char* vertexSource, const char* fragmentSource);
static ShaderProgram constructShaderProgramFromFile(const char* vertexPath, const char* fragmentPath);

struct ShaderProgramManager shaderProgramManager()
{
	// Yep, these shader files im including have been "stringified", so the include statements
	// basically just paste in a big string literal. pretty nice
	const char* canvasShaderSource[2] = {
		#include "../../../shaders/grid-engine-canvas-shader.vert"
		,
		#include "../../../shaders/grid-engine-canvas-shader.frag"
	};
	const char* borderShaderSource[2] = {
		#include "../../../shaders/grid-engine-border-shader.vert"
		,
		#include "../../../shaders/grid-engine-border-shader.frag"
	};

	struct ShaderProgramManager shaderProgramManager;
	shaderProgramManager.canvas = constructShaderProgramFromString(canvasShaderSource[0], canvasShaderSource[1]);
	shaderProgramManager.border = constructShaderProgramFromString(borderShaderSource[0], borderShaderSource[1]);

	shaderProgramManager.uniforms.canvasGridUnitCnt = glGetUniformLocation(shaderProgramManager.canvas, "gridUnitCnt");
	shaderProgramManager.uniforms.borderColor = glGetUniformLocation(shaderProgramManager.border, "borderColor");
	shaderProgramManager.uniforms.canvasOriginInBorderShaderNDC = glGetUniformLocation(shaderProgramManager.border, "aCanvasBottomLeftCoordsInNDC");

	return shaderProgramManager;
}
void shaderProgramManagerDestroy(struct ShaderProgramManager* shaderProgramManager)
{
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "Destroying ShaderProgramManager...\n");
	glDeleteProgram(shaderProgramManager->canvas);
	glDeleteProgram(shaderProgramManager->border);
}

void shaderProgramManagerSetGridUnitCntUniform(struct ShaderProgramManager* const shaderProgramManager, const uVec2 gridUnitCnt)
{
	glUseProgram(shaderProgramManager->canvas);
	glUniform2uiv(shaderProgramManager->uniforms.canvasGridUnitCnt, 1, gridUnitCnt.elements);
}
void shaderProgramManagerSetBorderColorUniform(struct ShaderProgramManager* const shaderProgramManager, const Color color)
{
	glUseProgram(shaderProgramManager->border);
	glUniform3fv(shaderProgramManager->uniforms.borderColor, 1, color.elements);
}
void shaderProgramManagerSetCanvasOriginInBorderShaderUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec2 coordsInNDC)
{
	glUseProgram(shaderProgramManager->border);
	glUniform2fv(shaderProgramManager->uniforms.canvasOriginInBorderShaderNDC, 1, coordsInNDC.elements);
}

void shaderProgramManagerSetCanvasUniforms(struct ShaderProgramManager* const shaderProgramManager, const struct CoordinateCanvas* const canvas)
{
	// this is all to get the canvas origin coords for the border shader (converted to NDC)
	Vec2 borderOrigin = canvasGetBorderOrigin(canvas);
	Vec2 borderSize = canvasGetBorderSize(canvas);
	Vec2 NDCrange = vec2(-1, 1);
	Vec2 canvasOriginInBorderShaderNDC = vec2(MAP_RANGE(canvas->origin.x, vec2(borderOrigin.x, borderOrigin.x + borderSize.width), NDCrange), 
						  MAP_RANGE(canvas->origin.y, vec2(borderOrigin.y, borderOrigin.y + borderSize.height), NDCrange));

	shaderProgramManagerSetBorderColorUniform(shaderProgramManager, canvas->border.color);
	shaderProgramManagerSetGridUnitCntUniform(shaderProgramManager, canvas->gridUnitCnt);
	shaderProgramManagerSetCanvasOriginInBorderShaderUniform(shaderProgramManager, canvasOriginInBorderShaderNDC);
}


static char* loadFileIntoString(const char* fileName)
{
	FILE* infile;
	unsigned long numOfBytesInFile, numOfBytesRead;
	char* string;

	infile = fopen(fileName, "r");
	if (!infile)
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s failed to open \n", fileName);
		return NULL;
	}
	
	fseek(infile, 0, SEEK_END);
	numOfBytesInFile = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	string = malloc((sizeof(char) * numOfBytesInFile) + 1); // +1 to make space for null termination
	if (!string)
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "memory allocation failure\n");
		abort();
	}

	// NOTE in linux, it is standard to end every file on an LF (like newline) character. fread reads this
	numOfBytesRead = fread(string, sizeof(char), numOfBytesInFile, infile);
	string[numOfBytesRead] = '\0'; // null terminating the string after the EOL

	fclose(infile);
	return string;
}
static void compileShader(unsigned int shaderID, int* success)
{
	char log[512];

	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, success);
	if (!(*success))
	{
		glGetShaderInfoLog(shaderID, 512, NULL, log);
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s", log);
	}
}

static ShaderProgram constructShaderProgramFromString(const char* vertexSource, const char* fragmentSource)
{
	unsigned int vertexShader, fragmentShader;
	ShaderProgram shaderProgram;
	int success;
	char log[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	compileShader(vertexShader, &success);
	if (!success) return 0;
	compileShader(fragmentShader, &success);
	if (!success) return 0;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s", log);
		return 0;
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
static ShaderProgram constructShaderProgramFromFile(const char* vertexPath, const char* fragmentPath)
{
	char *vertexSource, *fragmentSource;
	ShaderProgram shaderProgram;

	vertexSource = loadFileIntoString(vertexPath);
	fragmentSource = loadFileIntoString(fragmentPath);
	if (!vertexSource || !fragmentSource)
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "failed to load shader source files\n");
		return 0;
	}

	shaderProgram = constructShaderProgramFromString(vertexSource, fragmentSource);

	free(vertexSource);
	vertexSource = nullptr;
	free(fragmentSource);
	fragmentSource = nullptr;

	return shaderProgram;
}

