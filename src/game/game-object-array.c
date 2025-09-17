#include "game/game-object-array.h"

#include "game/game-object.h"
#include "utility/memory-util.h"
#include "utility/logging.h"
#include <stdlib.h>
#include <string.h>

struct GameObjectArray gameObjectArray()
{
	struct GameObjectArray gameObjectArray;
	gameObjectArray.num = 0;
	return gameObjectArray;
}

// @returns {bool} false if object array full
// NOTE: limited to this file scope since game objects typically must already be heap allocated before adding
static bool gameObjectArrayAdd(struct GameObjectArray* const gameObjectArray, struct GameObject* const gameObject)
{
	if (gameObjectArray->num == GRID_MAX_GAME_OBJECTS_AMT) return false;

	gameObjectArray->elements[gameObjectArray->num] = gameObject;
	gameObjectArray->num++;
	return true;
}

bool gameObjectArrayAddHeapCopy(struct GameObjectArray* const gameObjectArray, const struct GameObject* gameObject, const size_t sizeOfGameObject)
{
	// stores the user defined game objects on the heap and into the game object array
	struct GameObject* gameObjectOnHeap = malloc(sizeOfGameObject);
	memcpy(gameObjectOnHeap, gameObject, sizeOfGameObject);

	if (!gameObjectArrayAdd(gameObjectArray, gameObjectOnHeap))
	{
		LOG(GRID_LOGGING_WARN, __func__, __LINE__, "failed to add object %s, object array reached capacity\n", gameObject->id);
		free(gameObjectOnHeap);
		return false;
	}

	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "added object %s to object array\n", gameObject->id);
	gameObject = gameObjectOnHeap;
	return true;
}

bool gameObjectArrayRemove(struct GameObjectArray* const gameObjectArrayRemove, const char* id, struct GridEngine* const engine)
{
	struct GameObjectArray gameObjectsToRemove = gameObjectArray();
	int idx = 0;

	while (idx < gameObjectArrayRemove->num)
	{
		struct GameObject* gameObject = gameObjectArrayRemove->elements[idx];
		if (strcmp(gameObject->id, id) == 0)
		{
			LOG(GRID_LOGGING_FULL, __func__, __LINE__, "removing object %s from object array\n", gameObject->id);
			gameObjectArrayAdd(&gameObjectsToRemove, gameObject);

			// now that object to remove pointer is saved, it can be overwritten in the original GameObjectArray
			MEMMOVE_RANGE(&gameObjectArrayRemove->elements[idx], &gameObjectArrayRemove->elements[idx + 1], &gameObjectArrayRemove->elements[gameObjectArrayRemove->num]);
			gameObjectArrayRemove->num--;

			// continue here so that idx is not incremented since the next element has been shifted back one already
			continue;
		}
		idx++;
	}

	if (gameObjectsToRemove.num == 0) return false;
	gameObjectArrayDestroy(&gameObjectsToRemove, engine);

	return true;
}

struct GameObjectArray gameObjectArrayGet(const struct GameObjectArray* const gameObjectArrayToSearch, const char* id)
{
	struct GameObjectArray listOfMatchingGameObjects = gameObjectArray();
	if (!id) return listOfMatchingGameObjects;

	for (int idx = 0; idx < gameObjectArrayToSearch->num; idx++)
	{
		if (strcmp(gameObjectArrayToSearch->elements[idx]->id, id) == 0)
		{
			gameObjectArrayAdd(&listOfMatchingGameObjects, gameObjectArrayToSearch->elements[idx]);
		}
	}

	// up to the user to cast the game objects back to their actual extended type pointers
	return listOfMatchingGameObjects;
}

void gameObjectArrayDestroy(struct GameObjectArray* const gameObjectArray, struct GridEngine* const engine)
{
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "destroying game object array...\n");
	for (int idx = 0; idx < gameObjectArray->num; idx++)
	{
		struct GameObject* gameObject = gameObjectArray->elements[idx];
		LOG(GRID_LOGGING_FULL, __func__, __LINE__, "destroying object %s...\n", gameObject->id);

		gameObject->destroy(gameObject, engine);

		free(gameObject);
		gameObject = nullptr;
	}

	gameObjectArray->num = 0;
}
