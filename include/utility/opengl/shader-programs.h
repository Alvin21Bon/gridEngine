#pragma once

#include <lina/lina.h>

// ======================== MAKING SHADER PROGRAMS
typedef unsigned int ShaderProgram;

ShaderProgram constructShaderProgramFromString(const char* vertexSource, const char* fragmentSource);
ShaderProgram constructShaderProgramFromFile(const char* vertexPath, const char* fragmentPath);
// ======================== 

// ======================== MANAGING SHADER PROGRAMS
typedef int Uniform;

struct ShaderProgramManager {
	struct {
		ShaderProgram canvas;
		ShaderProgram border;
		ShaderProgram currentlyActive;
	} programs;

	struct {
		Uniform canvasGridUnitCnt;
		Uniform borderColor;
		Uniform canvasBottomLeftCoordsInNDC;
	} uniforms;
};

/*
 * These wrapper functions are provided so that the correct program is in use before setting
 * a uniform, and the use program exists so currentlyActive value gets set when setting programs
*/
struct ShaderProgramManager shaderProgramManager();
void shaderProgramManagerDestroy(struct ShaderProgramManager* shaderProgramManager);

void shaderProgramManagerUseProgram(struct ShaderProgramManager* const shaderProgramManager, ShaderProgram programToUse);
void shaderProgramManagerSetGridUnitCntUniform(struct ShaderProgramManager* const shaderProgramManager, const uVec2 gridUnitCnt);
void shaderProgramManagerSetBorderColorUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec3 color);
void shaderProgramManagerSetCanvasBottomLeftCoordsUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec2 coordsInNDC);
// ======================== 
