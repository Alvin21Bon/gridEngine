#include "../../../include/master-header.h"

struct CanvasRenderer canvasRenderer(const struct CoordinateCanvas* const canvas);
void canvasRendererDestory(struct CanvasRenderer* const canvasRenderer);

void canvasRendererUpdateSettings(struct CanvasRenderer* const canvasRenderer, const struct CoordinateCanvas* const canvas);
void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
void canvasRendererAllocateBuffers(struct CanvasRenderer* const canvasRenderer);
void canvasRendererFillBuffers(struct CanvasRenderer* const canvasRenderer, const struct CoordinateCanvas* const canvas);

void canvasRender(const struct CanvasRenderer* const canvasRenderer, struct ShaderProgramManager* const shaderProgramManager);
