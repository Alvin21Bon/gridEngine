#include "../include/grid-engine.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasBorder border(const Vec3 color, const float thickness)
{
	struct CanvasBorder border;
	border.color = color;
	border.thickness = thickness;
	border.isVisible = GL_TRUE;
	return border;
}

/* 
 * === SETTING FUNCTIONS ===
*/
void borderToggleVisibility(struct CanvasBorder* const border)
{
	border->isVisible = !border->isVisible;
}
