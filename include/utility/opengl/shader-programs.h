#pragma once

#include <lina/lina.h>
#include <utility/color.h>

struct CoordinateCanvas;

typedef int Uniform;
typedef unsigned int ShaderProgram;

struct ShaderProgramManager {
	ShaderProgram canvas;
	ShaderProgram border;

	struct {
		Uniform canvasGridUnitCnt;
		Uniform borderColor;
		Uniform canvasOriginInBorderShaderNDC;
	} uniforms;
};

struct ShaderProgramManager shaderProgramManager();
void shaderProgramManagerDestroy(struct ShaderProgramManager* shaderProgramManager);

void shaderProgramManagerSetGridUnitCntUniform(struct ShaderProgramManager* const shaderProgramManager, const uVec2 gridUnitCnt);
void shaderProgramManagerSetBorderColorUniform(struct ShaderProgramManager* const shaderProgramManager, const Color color);
void shaderProgramManagerSetCanvasOriginInBorderShaderUniform(struct ShaderProgramManager* const shaderProgramManager, const Vec2 coordsInNDC);

void shaderProgramManagerSetCanvasUniforms(struct ShaderProgramManager* const shaderProgramManager, const struct CoordinateCanvas* const canvas);

