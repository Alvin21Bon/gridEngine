#include "../../include/master-header.h"

struct GameObjectArray gameObjectArray()
{
	struct GameObjectArray gameObjectArray;
	gameObjectArray.num = 0;
	return gameObjectArray;
}

// @returns {bool} GRID_ENGINE_ERROR if object array full
// NOTE: limited to this file scope since game objects typically must already be heap allocated before adding
static bool gameObjectArrayAdd(struct GameObjectArray* const gameObjectArray, struct GameObject* const gameObject)
{
	if (gameObjectArray->num == GRID_MAX_GAME_OBJECTS_AMT) return GRID_ENGINE_ERROR;

	gameObjectArray->elements[gameObjectArray->num] = gameObject;
	gameObjectArray->num++;
	return GRID_ENGINE_SUCCESS;
}

bool gameObjectArrayAddHeapCopy(struct GameObjectArray* const gameObjectArray, const struct GameObject* const gameObject, const size_t sizeOfGameObject)
{
	// stores the user defined game objects on the heap and into the game object array
	struct GameObject* gameObjectOnHeap = malloc(sizeOfGameObject);
	memcpy(gameObjectOnHeap, gameObject, sizeOfGameObject);

	return gameObjectArrayAdd(gameObjectArray, gameObjectOnHeap);
}

bool gameObjectArrayRemove(struct GameObjectArray* const gameObjectArrayRemove, const char* id)
{
	struct GameObjectArray gameObjectsToRemove = gameObjectArray();
	int idx = 0;

	while (idx < gameObjectArrayRemove->num)
	{
		struct GameObject* gameObject = gameObjectArrayRemove->elements[idx];
		if (strcmp(gameObject->id, id) == 0)
		{
			gameObjectArrayAdd(&gameObjectsToRemove, gameObject);

			// now that object to remove pointer is saved, it can be overwritten in the original GameObjectArray
			MEMMOVE_RANGE(&gameObjectArrayRemove->e[idx], &gameObjectArrayRemove->e[idx + 1], &gameObjectArrayRemove->e[gameObjectArrayRemove->num]);
			gameObjectArrayRemove->num--;

			// continue here so that idx is not incremented since the next element has been shifted back one already
			continue;
		}
		idx++;
	}

	if (gameObjectsToRemove.num == 0) return GRID_ENGINE_ERROR;
	gameObjectArrayDestroy(&gameObjectsToRemove);

	return GRID_ENGINE_SUCCESS;
}

struct GameObjectArray gameObjectArrayGet(const struct GameObjectArray* const gameObjectArrayToSearch, const char* id)
{
	struct GameObjectArray listOfMatchingGameObjects = gameObjectArray();

	for (int idx = 0; idx < gameObjectArrayToSearch->num; idx++)
	{
		if (strcmp(gameObjectArrayToSearch->e[idx]->id, id) == 0)
		{
			gameObjectArrayAdd(&listOfMatchingGameObjects, gameObjectArrayToSearch->e[idx]);
		}
	}

	// up to the user to cast the game objects back to their actual extended type pointers
	return listOfMatchingGameObjects;
}

void gameObjectArrayDestroy(struct GameObjectArray* const gameObjectArray)
{
	for (int idx = 0; idx < gameObjectArray->num; idx++)
	{
		struct GameObject* gameObject = gameObjectArray->elements[idx];
		gameObject->destroy(gameObject);
		free(gameObject);
	}

	gameObjectArray->num = 0;
}
