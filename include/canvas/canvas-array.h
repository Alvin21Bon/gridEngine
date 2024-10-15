#pragma once

#include "canvas-types.h"
#include "../utility/shared.h"

struct CanvasArray {
	union {
		struct CoordinateCanvas* elements[GRID_MAX_CANVAS_AMT];
		struct CoordinateCanvas** e; // just a shorthand name for the array
	};

	size_t num;
};

struct CanvasArray canvasArray();

// Allocates canvas to the heap and stores it in canvas array
// @returns {bool} GRID_ENGINE_ERROR if canvas array full
bool canvasArrayAddHeapCopy(struct CanvasArray* const canvasArray, const struct CoordinateCanvas canvas);

// @returns {bool} GRID_ENGINE_ERROR if id not found
// NOTE: this destroys the removed canvas
bool canvasArrayRemove(struct CanvasArray* const canvasArray, const char* id);

// @returns array of canvases that match the ID
// WARN: the returned CanvasArray must not be destroyed, or the canvases inside be removed, or else the original CanvasArray will be affected
struct CanvasArray canvasArrayGet(const struct CanvasArray* const canvasArrayToSearch, const char* id);

// NOTE: the array is drawn back of array to the front, so the first element is on top
void canvasArrayDraw(struct CanvasArray* const canvasArray);

// NOTE: will always be called on CanvasArrays full of heap allocated canvases
void canvasArrayDestroy(struct CanvasArray* const canvasArray);
