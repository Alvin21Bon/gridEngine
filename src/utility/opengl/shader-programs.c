#include "../../../include/engine/grid-engine.h"

static char* loadFileIntoString(const char* fileName)
{
	FILE* infile;
	unsigned long numOfBytesInFile, numOfBytesRead;
	char* string;

	infile = fopen(fileName, "r");
	if (!infile)
	{
		printf("loadFileIntoString ERROR: file failed to open\n");
		return NULL;
	}
	
	fseek(infile, 0, SEEK_END);
	numOfBytesInFile = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	string = malloc((sizeof(char) * numOfBytesInFile) + 1); // +1 to make space for null termination
	if (!string)
	{
		printf("loadFileIntoString ERROR: memory allocation failure\n");
		return NULL;
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
		printf("%s", log);
	}
}

ShaderProgram constructShaderProgramFromString(const char* vertexSource, const char* fragmentSource)
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
		printf("%s", log);
		return 0;
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
ShaderProgram constructShaderProgramFromFile(const char* vertexPath, const char* fragmentPath)
{
	const char* vertexSource, *fragmentSource;
	ShaderProgram shaderProgram;

	vertexSource = loadFileIntoString(vertexPath);
	fragmentSource = loadFileIntoString(fragmentPath);
	if (!vertexSource || !fragmentSource)
	{
		printf("constructShaderProgramVF ERROR: failed to load shader source\n");
		return 0;
	}

	shaderProgram = constructShaderProgramFromString(vertexSource, fragmentSource);

	free((void*)vertexSource);
	free((void*)fragmentSource);

	return shaderProgram;
}


// =========================== MANAGING SHADER PROGRAMS

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
	shaderProgramManager.programs.canvas = constructShaderProgramFromString(canvasShaderSource[0], canvasShaderSource[1]);
	shaderProgramManager.programs.border = constructShaderProgramFromString(borderShaderSource[0], borderShaderSource[1]);
	shaderProgramManager.programs.currentlyActive = 0;

	shaderProgramManager.uniforms.canvasGridUnitCnt = glGetUniformLocation(shaderProgramManager.programs.canvas, "gridUnitCnt");
	shaderProgramManager.uniforms.borderColor = glGetUniformLocation(shaderProgramManager.programs.border, "borderColor");
	shaderProgramManager.uniforms.canvasBottomLeftCoordsInNDC = glGetUniformLocation(shaderProgramManager.programs.border, "aCanvasBottomLeftCoordsInNDC");

	return shaderProgramManager;
}
void shaderProgramManagerDestroy(struct ShaderProgramManager* shaderProgramManager)
{
	printf("Destroying ShaderProgramManager...\n");
	glDeleteProgram(shaderProgramManager->programs.canvas);
	glDeleteProgram(shaderProgramManager->programs.border);
	shaderProgramManager->programs.currentlyActive = 0;
}

void shaderProgramManagerUseProgram(struct ShaderProgramManager* const shaderProgramManager, ShaderProgram programToUse)
{
	glUseProgram(programToUse);
	shaderProgramManager->programs.currentlyActive = programToUse;
}
void shaderProgramManagerSetGridUnitCntUniform(struct ShaderProgramManager* const shaderProgramManager, uVec2 gridUnitCnt)
{
	// dont use wrapper so currentlyActive is not set
	glUseProgram(shaderProgramManager->programs.canvas);

	glUniform2uiv(shaderProgramManager->uniforms.canvasGridUnitCnt, 1, gridUnitCnt.elements);
	glUseProgram(shaderProgramManager->programs.currentlyActive);
}
void shaderProgramManagerSetBorderColorUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec3 color)
{
	// dont use wrapper so currentlyActive is not set
	glUseProgram(shaderProgramManager->programs.border);

	glUniform3fv(shaderProgramManager->uniforms.borderColor, 1, color.elements);
	glUseProgram(shaderProgramManager->programs.currentlyActive);
}
void shaderProgramManagerSetCanvasBottomLeftCoordsUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec2 coordsInNDC)
{
	// dont use wrapper so currentlyActive is not set
	glUseProgram(shaderProgramManager->programs.border);

	glUniform2fv(shaderProgramManager->uniforms.canvasBottomLeftCoordsInNDC, 1, coordsInNDC.elements);
	glUseProgram(shaderProgramManager->programs.currentlyActive);
}
