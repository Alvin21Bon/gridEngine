#include "utility/opengl/canvas-rendering.h"

struct CanvasRenderer canvasRenderer(struct CoordinateCanvas* const canvas);
void canvasUpdateRenderer(struct CoordinateCanvas* const canvas);
void canvasRendererDestroy(struct CanvasRenderer* const canvasRenderer);

void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
void canvasAllocateRendererVertexBuffer(struct CoordinateCanvas* const canvas);
void canvasUpdateRendererVertexBuffer(struct CoordinateCanvas* const canvas);

void canvasDraw(struct CoordinateCanvas* const canvas, struct ShaderProgramManager* const shaderProgramManager);
