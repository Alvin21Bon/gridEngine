#pragma once

#include "utility/constants.h"
#include <stddef.h>

struct GameObject;

struct GameObjectArray {
	union {
		struct GameObject* elements[GRID_MAX_GAME_OBJECTS_AMT];
		struct GameObject** e; // just a shorthand name for the array
	};

	size_t num;
};

struct GameObjectArray gameObjectArray();

// Allocates objects to the heap and stores it in object array
// @param {sizeOfGameObject} since GameObjects extend the base GameObject struct, this is to know how much space to allocate
// @param {gameObject} pointer gets changed to point to new game object on heap
// @returns {bool} false if game object array full
bool gameObjectArrayAddHeapCopy(struct GameObjectArray* const gameObjectArray, const struct GameObject* gameObject, const size_t sizeOfGameObject);

// @returns {bool} false if id not found
// NOTE: this destroys the removed game objects
bool gameObjectArrayRemove(struct GameObjectArray* const gameObjectArrayRemove, const char* id);

// @returns array of game objects that match the ID
// NOTE: up to the user to check if gameObjectArrat has a num of 0 if id not found
// WARN: any altering of the GameObjects in the returned array, such as destroying/freeing, will impact the original GameObjectArray
struct GameObjectArray gameObjectArrayGet(const struct GameObjectArray* const gameObjectArrayToSearch, const char* id);

// NOTE: calling the update/draw functions of all game objects function will be made for the GameState struct

// NOTE: will always be called on GameObjectArrays full of heap allocated game objects
void gameObjectArrayDestroy(struct GameObjectArray* const gameObjectArray);
