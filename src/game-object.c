#include "../include/grid-engine.h"

int defaultObjectUpdateFunction(struct GameObject* const object, struct GameState* const game) {return GRID_ENGINE_SUCCESS;}
void defaultObjectDrawFunction(const struct GameObject* const object) {return;}
void defaultObjectDestoryFunction(struct GameObject* object) {return;}

struct GameObject gameObject(const uint id, const uint canvasId)
{
	struct GameObject gameObject;
	gameObject.id = id;
	gameObject.canvasId = canvasId;
	gameObject.update = defaultObjectUpdateFunction;
	gameObject.draw = defaultObjectDrawFunction;
	gameObject.destory = defaultObjectDestoryFunction;

	return gameObject;
}
void gameObjectAttachUpdateFunction(struct GameObject* const object, int (*updateFunction)(struct GameObject* const, struct GameState* const))
{
	object->update = updateFunction;
}
void gameObjectAttachDrawFunction(struct GameObject* const object, void (*drawFunction)(const struct GameObject* const))
{
	object->draw = drawFunction;
}
void gameObjectAttachDestoryFunction(struct GameObject* const object, void (*destoryFunction)(struct GameObject*))
{
	object->destory = destoryFunction;
}
