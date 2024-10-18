#pragma once

struct CoordinateCanvas;
struct GameState;

/* 
 *
 * GameObject will be "inhereted" by users to create their own extended game objects. These will be stored in the 
 * GameState by casting the pointer to one of GameObject. This will allow for the engine to call the update() and
 * draw() functions within the game loop's update function.
 *
 * NOTE: typically, the update function will make any changes to the user defined game object data and ensure that the
 * 	 game object is in a valid state. Any call to the GameObject draw call assumes the game object is in a valid
 * 	 state. the draw function will modify the canvas accordingly.
 *
 * NOTE: the destroyer function is called when the game object is removed from the game state so necessary cleanup for the user defined object can be done
 *
 * NOTE: in the draw calls, it is GUARANTEED the target canvas exists since the engine checks this before the draw function is called
*/
struct GameObject {
	char* id;
	char* canvasId; // stores ID since storing a pointer to the canvas could be dangerous if canvas is already removed from the renderingArray
	int (*update)(struct GameObject* const, struct GameState* const);
	void (*draw)(const struct GameObject* const, struct CoordinateCanvas* const);
	void (*destroy)(struct GameObject*);
};

struct GameObject gameObject(const char* id, const char* canvasId);
void gameObjectAttachUpdateFunction(struct GameObject* const object, int (*updateFunction)(struct GameObject* const, struct GameState* const));
void gameObjectAttachDrawFunction(struct GameObject* const object, void (*drawFunction)(const struct GameObject* const, struct CoordinateCanvas* const));
void gameObjectAttachDestroyFunction(struct GameObject* const object, void (*destroyFunction)(struct GameObject*));

int defaultObjectUpdateFunction(struct GameObject* const object, struct GameState* const game);
void defaultObjectDrawFunction(const struct GameObject* const object, struct CoordinateCanvas* const canvas);
void defaultObjectDestroyFunction(struct GameObject* object);

