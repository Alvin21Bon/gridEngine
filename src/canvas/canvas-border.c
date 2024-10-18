#include "canvas/canvas-border.h"

#include "utility/math-util.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasBorder border(const Vec3 color, const float thickness)
{
	struct CanvasBorder border;
	borderSetColor(&border, color);
	border.thickness = thickness;
	border.isVisible = true;
	return border;
}

/* 
 * === SETTING FUNCTIONS ===
*/
void borderToggleVisibility(struct CanvasBorder* const border)
{
	border->isVisible = !border->isVisible;
}
void borderSetColor(struct CanvasBorder* const border, const Vec3 color)
{
	Vec2 originalRange = vec2(0, 255);
	Vec2 newRange = vec2(0, 1);
	Vec3 mappedColor = vec3(MAP_RANGE(color.r, originalRange, newRange),
			 	MAP_RANGE(color.g, originalRange, newRange),
			 	MAP_RANGE(color.b, originalRange, newRange));
	border->color = mappedColor;
}
