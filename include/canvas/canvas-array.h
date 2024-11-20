#pragma once

#include "utility/constants.h"
#include <stddef.h>

struct CoordinateCanvas;
struct ShaderProgramManager;

struct CanvasArray {
	union {
		struct CoordinateCanvas* elements[GRID_MAX_CANVAS_AMT];
		struct CoordinateCanvas** e; // just a shorthand name for the array
	};

	size_t num;
};

struct CanvasArray canvasArray();

// Allocates canvas to the heap and stores it in canvas array
// @param {canvas} gets modified to point to new canvas on heap
// @returns {bool} false if canvas array full
bool canvasArrayAddHeapCopy(struct CanvasArray* const canvasArray, const struct CoordinateCanvas* canvas);

// @returns {bool} false if id not found
// NOTE: this destroys the removed canvases
bool canvasArrayRemove(struct CanvasArray* const canvasArrayRemove, const char* id);

// @returns array of canvases that match the ID
// NOTE: up to the user to check if array has a num of 0
// WARN: the returned CanvasArray must not be destroyed, or the canvases inside be removed, or else the original CanvasArray will be affected
struct CanvasArray canvasArrayGet(const struct CanvasArray* const canvasArrayToSearch, const char* id);

void canvasArrayDraw(struct CanvasArray* const canvasArray, struct ShaderProgramManager* const shaderProgramManager);

// NOTE: will always be called on CanvasArrays full of heap allocated canvases
void canvasArrayDestroy(struct CanvasArray* const canvasArray);
