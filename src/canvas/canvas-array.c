#include "canvas/canvas-array.h"

#include "canvas/coordinate-canvas.h"
#include "utility/opengl/canvas-rendering.h"
#include "utility/memory-util.h"
#include "utility/logging.h"
#include <stdlib.h>
#include <string.h>

struct CanvasArray canvasArray()
{
	struct CanvasArray canvasArray;
	canvasArray.num = 0;
	return canvasArray;
}

// @returns {bool} false if canvas array full
// NOTE: limited to this file scope since CanvasArray elements must typically always be on heap
static bool canvasArrayAdd(struct CanvasArray* const canvasArray, struct CoordinateCanvas* const canvas)
{
	if (canvasArray->num == GRID_MAX_CANVAS_AMT) return false;

	canvasArray->elements[canvasArray->num] = canvas;
	canvasArray->num++;
	return true;
}

bool canvasArrayAddHeapCopy(struct CanvasArray* const canvasArray, const struct CoordinateCanvas* canvas)
{
	struct CoordinateCanvas* canvasOnHeap = malloc(sizeof(struct CoordinateCanvas));
	*canvasOnHeap = *canvas;

	if (!canvasArrayAdd(canvasArray, canvasOnHeap))
	{
		LOG(GRID_LOGGING_WARN, __func__, __LINE__, "failed to add canvas %s, canvas array reached max capacity\n", canvas->id);
		free(canvasOnHeap);
		return false;
	}

	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "added canvas %s to canvas array\n", canvas->id);
	canvas = canvasOnHeap;
	return true;
}

bool canvasArrayRemove(struct CanvasArray* const canvasArrayRemove, const char* id)
{
	// canvasArrayGet not used since I need to also get the indices of matching canvases
	struct CanvasArray canvasesToRemove = canvasArray();
	int idx = 0;

	while (idx < canvasArrayRemove->num)
	{
		struct CoordinateCanvas* canvas = canvasArrayRemove->elements[idx];
		if (strcmp(canvas->id, id) == 0)
		{
			LOG(GRID_LOGGING_FULL, __func__, __LINE__, "removing canvas %s from canvas array\n", canvas->id);
			canvasArrayAdd(&canvasesToRemove, canvas);

			// now that canvas to remove pointer is saved, we can safetly alter the original CanvasArray
			MEMMOVE_RANGE(&canvasArrayRemove->elements[idx], &canvasArrayRemove->elements[idx + 1], &canvasArrayRemove->elements[canvasArrayRemove->num]);
			canvasArrayRemove->num--;

			// continue here so that idx is not incremented since the next element has been shifted back one already
			continue;
		}
		idx++;
	}

	if (canvasesToRemove.num == 0) return false;
	canvasArrayDestroy(&canvasesToRemove);

	return true;
}

struct CanvasArray canvasArrayGet(const struct CanvasArray* const canvasArrayToSearch, const char* id)
{
	struct CanvasArray listOfMatchingCanvases = canvasArray();

	for (int idx = 0; idx < canvasArrayToSearch->num; idx++)
	{
		if (strcmp(canvasArrayToSearch->elements[idx]->id, id) == 0)
		{
			canvasArrayAdd(&listOfMatchingCanvases, canvasArrayToSearch->elements[idx]);
		}
	}

	return listOfMatchingCanvases;
}

void canvasArrayDraw(struct CanvasArray* const canvasArray)
{
	for (int idx = 0; idx < canvasArray->num; idx++)
	{
		canvasDraw(canvasArray->elements[idx]);
	}
}

void canvasArrayDestroy(struct CanvasArray* const canvasArray)
{
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "destroying canvas array...\n");
	for (int idx = 0; idx < canvasArray->num; idx++)
	{
		struct CoordinateCanvas* canvas = canvasArray->elements[idx];
		canvasDestroy(canvas);
		free(canvas);
		canvas = nullptr;
	}

	canvasArray->num = 0;
}

