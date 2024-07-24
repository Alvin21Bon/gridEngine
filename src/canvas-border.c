#include "../include/grid-engine.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
const struct CanvasBorder border(const Vec3 color, const float thickness)
{
	struct CanvasBorder border = {GL_TRUE, color, thickness};
	return border;
}

/* 
 * === SETTING FUNCTIONS ===
*/
void borderToggleVisibility(struct CanvasBorder* const border)
{
	border->isVisible = !border->isVisible;
}
