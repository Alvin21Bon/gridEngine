#include "canvas/canvas-border.h"

struct CanvasBorder border(const Color color, const float thickness)
{
	struct CanvasBorder border;
	border.color = color;
	border.thickness = thickness;
	border.isVisible = true;
	return border;
}

void borderToggleVisibility(struct CanvasBorder* const border)
{
	border->isVisible = !border->isVisible;
}
