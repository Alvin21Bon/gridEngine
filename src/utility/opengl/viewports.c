#include "utility/opengl/viewports.h"

#include "utility/opengl/glad.h"

void viewportUse(const struct Viewport* const viewport)
{
	glViewport(viewport->origin.x, viewport->origin.y, viewport->size.width, viewport->size.height);
}
