#pragma once

#include <lina/lina.h>

struct CoordinateCanvas;

// ======================== MAKING SHADER PROGRAMS
typedef unsigned int ShaderProgram;

ShaderProgram constructShaderProgramFromString(const char* vertexSource, const char* fragmentSource);
ShaderProgram constructShaderProgramFromFile(const char* vertexPath, const char* fragmentPath);
// ======================== 

// ======================== MANAGING SHADER PROGRAMS
typedef int Uniform;

struct ShaderProgramManager {
	ShaderProgram canvas;
	ShaderProgram border;

	struct {
		Uniform canvasGridUnitCnt;
		Uniform borderColor;
		Uniform canvasOriginInBorderShaderNDC;
	} uniforms;
};

/*
 * These wrapper functions are provided so that the correct program is in use before setting
 * a uniform
*/
struct ShaderProgramManager shaderProgramManager();
void shaderProgramManagerDestroy(struct ShaderProgramManager* shaderProgramManager);

void shaderProgramManagerSetGridUnitCntUniform(struct ShaderProgramManager* const shaderProgramManager, const uVec2 gridUnitCnt);
void shaderProgramManagerSetBorderColorUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec3 color);
void shaderProgramManagerSetCanvasOriginInBorderShaderUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec2 coordsInNDC);

void shaderProgramManagerSetCanvasUniforms(struct ShaderProgramManager* const shaderProgramManager, const struct CoordinateCanvas* const canvas);
// ======================== 
