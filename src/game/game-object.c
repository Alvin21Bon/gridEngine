#include "game/game-object.h"

#include "engine/grid-engine-states.h"

int defaultObjectUpdateFunction(struct GameObject* const object, struct GameState* const game) {return GRID_ENGINE_SUCCESS;}
void defaultObjectDrawFunction(const struct GameObject* const object, struct CoordinateCanvas* const canvas) {return;}
void defaultObjectDestroyFunction(struct GameObject* object) {return;}

struct GameObject gameObject(const char* id, const char* canvasId)
{
	struct GameObject gameObject;
	gameObject.id = id;
	gameObject.canvasId = canvasId;
	gameObject.update = defaultObjectUpdateFunction;
	gameObject.draw = defaultObjectDrawFunction;
	gameObject.destroy = defaultObjectDestroyFunction;

	return gameObject;
}
void gameObjectAttachUpdateFunction(struct GameObject* const object, int (*updateFunction)(struct GameObject* const, struct GameState* const))
{
	object->update = updateFunction;
}
void gameObjectAttachDrawFunction(struct GameObject* const object, void (*drawFunction)(const struct GameObject* const, struct CoordinateCanvas* const))
{
	object->draw = drawFunction;
}
void gameObjectAttachDestroyFunction(struct GameObject* const object, void (*destroyFunction)(struct GameObject*))
{
	object->destroy = destroyFunction;
}
