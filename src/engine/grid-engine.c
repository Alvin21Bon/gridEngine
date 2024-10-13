#include "../../include/engine/grid-engine.h"

GLFWwindow* gridEngineCreateAndSetupWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "hyprland:float", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	return window;
}

ShaderProgram gridEngineGenerateCanvasProgram()
{
	const char* vertexSource = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPixelColor;\n"
		"layout (location = 1) in uint isPixelVisible;\n"
		"layout (location = 2) in vec2 vertexPosition;\n"
		"uniform uvec2 gridUnitCnt;\n"
		"out vec3 pixelColor;\n"
		"void main()\n"
		"{\n"
			"uint columnPos = uint(gl_InstanceID) / gridUnitCnt.y;\n"
			"uint rowPos = uint(gl_InstanceID) % gridUnitCnt.y;\n"
			"vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"
			"float gridUnitWidthNDC = 2.0 / gridUnitCnt.x;\n"
			"float gridUnitHeightNDC = 2.0 / gridUnitCnt.y;\n"
			"vec2 gridOffsetAmount = vec2(columnPos * gridUnitWidthNDC, rowPos * gridUnitHeightNDC);\n"
			"vec2 translatedPosition = viewportBottomLeftCoords + gridOffsetAmount + vertexPosition;\n"
			"bool boolIsPixelVisible = bool(isPixelVisible);\n"
			"translatedPosition = translatedPosition + (int(!boolIsPixelVisible) * 100);\n"
			"gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"
			"pixelColor = aPixelColor;\n"
		"}\0"
		;

	const char* fragmentSource = 
		"#version 330 core\n"
		"in vec3 pixelColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(pixelColor, 1.0);\n"
		"}\0"
		;

	return constructShaderProgramFromString(vertexSource, fragmentSource);
}
ShaderProgram gridEngineGenerateBorderProgram()
{
	const char* vertexSource = 
		"#version 330 core\n"
		"layout (location = 2) in vec2 vertexPosition;\n"
		"uniform vec3 borderColor;\n"
		"uniform vec2 aCanvasBottomLeftCoordsInNDC;\n" 
		"out vec3 pixelColor;\n"
		"out vec2 canvasBottomLeftCoordsInNDC;\n"
		"out vec2 canvasTopRightCoordsInNDC;\n"
		"out vec2 pixelCoordsInNDC;\n" 
		"void main()\n"
		"{\n"
			"vec2 translatedPosition = vertexPosition * 1000000;\n"
			"vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"
			"translatedPosition = translatedPosition + viewportBottomLeftCoords;\n"
			"gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"
			"pixelCoordsInNDC = gl_Position.xy;\n"
			"canvasBottomLeftCoordsInNDC = aCanvasBottomLeftCoordsInNDC;\n"
			"canvasTopRightCoordsInNDC = canvasBottomLeftCoordsInNDC * -1;\n"
			"pixelColor = borderColor;\n"
		"}\0"
		;

	const char* fragmentSource = 
		"#version 330 core\n"
		"in vec3 pixelColor;\n"
		"in vec2 canvasBottomLeftCoordsInNDC;\n"
		"in vec2 canvasTopRightCoordsInNDC;\n"
		"in vec2 pixelCoordsInNDC;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"if (all(greaterThan(pixelCoordsInNDC, canvasBottomLeftCoordsInNDC)) && all(lessThan(pixelCoordsInNDC, canvasTopRightCoordsInNDC))) discard;\n"
			"FragColor = vec4(pixelColor, 1.0);\n"
		"}\0"
		;

	return constructShaderProgramFromString(vertexSource, fragmentSource);
}

void gridEngineTerminate(struct GameState* game)
{
	gameStateDestroy(game);
	glfwTerminate();
}

// these functions add support for moving and scaling canvases with the mouse. this is performed before the main update state stuff
static void gridEngineMoveCanvasOnMouseInteraction(struct GameState* const game)
{
	Vec2 deltaCursorMovement = vec2(game->inputData.cursorPos.x - game->previousInputData.cursorPos.x, game->inputData.cursorPos.y - game->previousInputData.cursorPos.y);
	canvasTranslate(game->targetCanvasForMouseInteractions, deltaCursorMovement);
}
static void gridEngineHorizontalScaleCanvasOnMouseInteraction(struct GameState* const game)
{
	struct CoordinateCanvas* canvas = game->targetCanvasForMouseInteractions;
	float deltaCursorX = game->inputData.cursorPos.x - game->previousInputData.cursorPos.x;

	if (game->gameInfo.cursorState == GRID_CURSOR_RESIZING_LEFT)
	{
		canvas->origin.x += deltaCursorX;
		canvasSetSize(canvas, vec2(canvas->size.width - deltaCursorX, canvas->size.height));
	}
	else // GRID_CURSOR_RESIZING_RIGHT
	{
		canvasSetSize(canvas, vec2(canvas->size.width + deltaCursorX, canvas->size.height));
	}
}
static void gridEngineVerticalScaleCanvasOnMouseInteraction(struct GameState* const game)
{
	struct CoordinateCanvas* canvas = game->targetCanvasForMouseInteractions;
	float deltaCursorY = game->inputData.cursorPos.y - game->previousInputData.cursorPos.y;

	if (game->gameInfo.cursorState == GRID_CURSOR_RESIZING_BELOW)
	{
		canvas->origin.y += deltaCursorY;
		canvasSetSize(canvas, vec2(canvas->size.width, canvas->size.height - deltaCursorY));
	}
	else // GRID_CURSOR_RESIZING_ABOVE
	{
		canvasSetSize(canvas, vec2(canvas->size.width, canvas->size.height + deltaCursorY));
	}
}
static struct CoordinateCanvas* gridEngineGetCanvasUnderCursorAndUpdateCursorState(struct GameState* const game)
{
	// all state updating is paused when the left mouse button is held down. this allows for the cursor state to 
	// stay persistent as long as the mouse button is held down, whatever it was before the left mouse button was pressed
	//
	// there is a one frame window where the cursor state still updates just so the state is in its most recent state
	// when the state updating pauses. This is also so there is a one frame window where this function can return the 
	// canvas so it can be set as the target canvas 
	if (game->inputData.mouseFlags[GLFW_MOUSE_BUTTON_LEFT] && game->previousInputData.mouseFlags[GLFW_MOUSE_BUTTON_LEFT]) return NULL;

	const int PIXEL_THICKNESS_OF_MIN_RESIZE_DISTANCE = 10;
	struct CoordinateCanvas* canvas;
	Vec2 canvasTopRightPos;
	Vec2 minResizeBoxOrigin;
	Vec2 minResizeBoxTopRightPos;
	Vec2 canvasBorderOrigin;
	Vec2 canvasBorderSize;
	Vec2 canvasBorderTopRightPos;
	Vec2 cursorPos = vec2(game->inputData.cursorPos.x, game->inputData.cursorPos.y);

	// loop backwards so the canvases on top are checked first
	for (int idx = game->gameInfo.numCanvases - 1; idx >= 0; idx--)
	{
		canvas = game->canvasRenderingArray[idx];
		canvasTopRightPos = vec2Add(canvas->origin, canvas->size);

		minResizeBoxOrigin = vec2(canvas->origin.x - PIXEL_THICKNESS_OF_MIN_RESIZE_DISTANCE, canvas->origin.y - PIXEL_THICKNESS_OF_MIN_RESIZE_DISTANCE);
		minResizeBoxTopRightPos = vec2(canvasTopRightPos.x + PIXEL_THICKNESS_OF_MIN_RESIZE_DISTANCE, canvasTopRightPos.y + PIXEL_THICKNESS_OF_MIN_RESIZE_DISTANCE);

		canvasBorderOrigin = canvasGetBorderOrigin(canvas);
		canvasBorderSize = canvasGetBorderSize(canvas);
		canvasBorderTopRightPos = vec2Add(canvasBorderOrigin, canvasBorderSize);

		// if the cursor is right over the canvas (moving)
		if (IS_IN_BOXED_RANGE(cursorPos, canvas->origin, canvasTopRightPos))
		{
			if (canvas->isMovableWithMouse)
			{
				gameStateSetCursorState(game, GRID_CURSOR_MOVING);
				return canvas;
			}
		}
		// if not in canvas, but in a small box around the canvas/over the border (resizing)
		else if (IS_IN_BOXED_RANGE(cursorPos, minResizeBoxOrigin, minResizeBoxTopRightPos) || IS_IN_BOXED_RANGE(cursorPos, canvasBorderOrigin, canvasBorderTopRightPos))
		{
			if (canvas->isScalableWithMouse)
			{
				// check for if on every side of the canvas
				if (cursorPos.x < canvas->origin.x) gameStateSetCursorState(game, GRID_CURSOR_RESIZING_LEFT);
				else if (cursorPos.x > canvasTopRightPos.x) gameStateSetCursorState(game, GRID_CURSOR_RESIZING_RIGHT); 
				else if (cursorPos.y < canvas->origin.y) gameStateSetCursorState(game, GRID_CURSOR_RESIZING_BELOW); 
				else if (cursorPos.y > canvasTopRightPos.y) gameStateSetCursorState(game, GRID_CURSOR_RESIZING_ABOVE); 

				return canvas;
			}
		}
	}

	gameStateSetCursorState(game, GRID_CURSOR_DEFAULT);
	return NULL;
}
static void performAnyCanvasMouseInteractions(struct GameState* const game)
{
	struct CoordinateCanvas* canvas = gridEngineGetCanvasUnderCursorAndUpdateCursorState(game);
	if (game->gameInfo.cursorState == GRID_CURSOR_DEFAULT || !game->inputData.mouseFlags[GLFW_MOUSE_BUTTON_LEFT])
	{
		game->targetCanvasForMouseInteractions = NULL;
		return;
	}
	// if function got past to this point, there are some gaurantees
	// 	1. mouse is in position to do some interaction with a canvas
	// 	2. for one frame when the left mouse button is first hit, the canvas that is returned is the target canvas. the fact that the 
	// 	   target canvas still exists (not a dangling pointer by manually removal in later update functions) is not gauranteed for the 
	// 	   whole event, so much check
	// 	3. left mouse button is being pressed this frame
	
	bool wasLeftMouseButtonJustPressed = !game->previousInputData.mouseFlags[GLFW_MOUSE_BUTTON_LEFT]; 
	if (wasLeftMouseButtonJustPressed)
	{
		game->targetCanvasForMouseInteractions = canvas;
	}

	if (game->targetCanvasForMouseInteractions != NULL)
	{
		// these functions are always calls depending on the cursor state, so anything that is true of the cursor state (like being over a canvas) is true in the function
		switch (game->gameInfo.cursorState)
		{
			case GRID_CURSOR_MOVING:
				gridEngineMoveCanvasOnMouseInteraction(game);
				break;
			case GRID_CURSOR_RESIZING_LEFT:
			case GRID_CURSOR_RESIZING_RIGHT:
				gridEngineHorizontalScaleCanvasOnMouseInteraction(game);
				break;
			case GRID_CURSOR_RESIZING_BELOW:
			case GRID_CURSOR_RESIZING_ABOVE:
				gridEngineVerticalScaleCanvasOnMouseInteraction(game);
				break;
			default:
				printf("GRID_CANVAS_MOUSE_INTERACTIONS ERROR: loose int values somehow entered the switch. FIX.\n");
				break;
        	}
	}
}

// used in the main game loop
static void gridEngineRender(struct GameState* game)
{
	glClear(GL_COLOR_BUFFER_BIT);
	gameStateDraw(game);
	glfwSwapBuffers(game->gameInfo.window);
}
static int gridEngineUpdateGameState(struct GameState* game)
{
	game->previousInputData = game->inputData;
	glfwPollEvents();

	performAnyCanvasMouseInteractions(game);

	// PRE UPDATE
	if (game->preUpdate(game) == GRID_ENGINE_ERROR) 
	{
		printf("GRID_GAME_STATE_UPDATE ERROR: GameState pre-update returned an error\n");
		return GRID_ENGINE_ERROR;
	}

	// PERFORM ALL GAME OBJECT UPDATE AND DRAW FUNCTIONS
	struct GameObject* object;
	struct CoordinateCanvas* canvasToDrawOn;
	for (int idx = 0; idx < game->gameInfo.numObjects; idx++)
	{
		object = game->objectArray[idx];

		// object function function
		if (object->update(object, game) == GRID_ENGINE_ERROR) 
		{
			printf("GRID_GAME_STATE_UPDATE ERROR: object with ID %d update returned an error\n", object->id);
			return GRID_ENGINE_ERROR;
		}

		// object draw (on canvas) function
		canvasToDrawOn = gameStateGetCanvas(game, object->canvasId);
		if (canvasToDrawOn != NULL) 
			object->draw(object, canvasToDrawOn);
		// ^^^ this error checking is done here since EVERY draw call would need to have this anyway. So in the draw calls, it is guaranteed the canvas is in the canvas array
	}

	// POST UPDATE
	if (game->postUpdate(game) == GRID_ENGINE_ERROR) 
	{
		printf("GRID_GAME_STATE_UPDATE ERROR: GameState post-update returned an error\n");
		return GRID_ENGINE_ERROR;
	}
	
	return GRID_ENGINE_SUCCESS;
}
static void waitOnMaxFPS(struct GameState* game)
{
	double timeAtStartOfGameLoop = game->timeData.currentTime;
	double currentTime = glfwGetTime();
	double currentDeltaTime = currentTime - timeAtStartOfGameLoop;
	double currentFPS = 1.0 / currentDeltaTime;

	while (currentFPS > GRID_GAME_MAX_FPS)
	{
		currentTime = glfwGetTime();
		currentDeltaTime = currentTime - timeAtStartOfGameLoop;
		currentFPS = 1.0 / currentDeltaTime;
	}
}

int gridEngineGameLoop(struct GameState* game)
{
	while (!glfwWindowShouldClose(game->gameInfo.window))
	{
		gameStateUpdateTime(game);

		// ========================================
		// CORE OF GAME LOOP
		if (gridEngineUpdateGameState(game) == GRID_ENGINE_ERROR)
		{
			printf("GRID_GAME_LOOP ERROR: GameState update returned an error\n");
			return GRID_ENGINE_ERROR;
		}
		gridEngineRender(game);
		// ========================================

		// puts the loop in a spin cycle until FPS is not above defined max FPS
		waitOnMaxFPS(game);
	}

	return GRID_ENGINE_SUCCESS;
}
