#include "../../../include/master-header.h"

void viewportUse(const struct Viewport* const viewport)
{
	glViewport(viewport->origin.x, viewport->origin.y, viewport->size.width, viewport->size.height);
}
