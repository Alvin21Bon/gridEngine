#include "../../include/engine/grid-engine.h"

struct CanvasArray canvasArray()
{
	struct CanvasArray canvasArray;
	canvasArray.num = 0;
	return canvasArray;
}

// @returns {bool} GRID_ENGINE_ERROR if canvas array full
// NOTE: limited to this file scope since CanvasArray elements must typically always be on heap
static bool canvasArrayAdd(struct CanvasArray* const canvasArray, struct CoordinateCanvas* canvas)
{
	if (canvasArray->num == GRID_MAX_CANVAS_AMT) return GRID_ENGINE_ERROR;

	canvasArray->elements[canvasArray->num] = canvas;
	canvasArray->num++;
	return GRID_ENGINE_SUCCESS;
}

bool canvasArrayAddHeapCopy(struct CanvasArray* const canvasArray, const struct CoordinateCanvas canvas)
{
	struct CoordinateCanvas* canvasOnHeap = malloc(sizeof(struct CoordinateCanvas));
	*canvasOnHeap = canvas;

	return canvasArrayAdd(canvasArray, canvasOnHeap);
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
			canvasArrayAdd(&canvasesToRemove, canvas);

			// now that canvas to remove pointer is saved, we can safetly alter the original CanvasArray
			MEMMOVE_RANGE(&canvasArrayRemove->e[idx], &canvasArrayRemove->e[idx + 1], &canvasArrayRemove->e[canvasArrayRemove->num]);
			canvasArrayRemove->num--;

			// continue here so that idx is not incremented since the next element has been shifted back one already
			continue;
		}
		idx++;
	}

	if (canvasesToRemove.num == 0) return GRID_ENGINE_ERROR;
	canvasArrayDestroy(&canvasesToRemove);

	return GRID_ENGINE_SUCCESS;
}

struct CanvasArray canvasArrayGet(const struct CanvasArray* const canvasArrayToSearch, const char* id)
{
	struct CanvasArray listOfMatchingCanvases = canvasArray();

	for (int idx = 0; idx < canvasArrayToSearch->num; idx++)
	{
		if (strcmp(canvasArrayToSearch->e[idx]->id, id) == 0)
		{
			canvasArrayAdd(&listOfMatchingCanvases, canvasArrayToSearch->e[idx]);
		}
	}

	return listOfMatchingCanvases;
}

// TODO: implement after engine refactor and gameState is decoupled from canvas draw
void canvasArrayDraw(struct CanvasArray* const canvasArray);

void canvasArrayDestroy(struct CanvasArray* const canvasArray)
{
	for (int idx = 0; idx < canvasArray->num; idx++)
	{
		struct CoordinateCanvas* canvas = canvasArray->elements[idx];
		canvasDestroy(canvas);
		free(canvas);
	}

	canvasArray->num = 0;
}
