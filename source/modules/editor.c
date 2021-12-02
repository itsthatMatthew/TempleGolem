#include "./../debugmalloc.h"
#include "./gamestructs.h"
#include "./textmanager.h"
#include "./engines/graphics.h"
#include <stdbool.h>

// Button list
/* SAVE LOAD AIR WATER LAVA WALL FLOOR START FINISH GEM */

// constant initialization
#define BUTTONCOUNT 9

// All the blocktypes for determining which has to be placed on the field
static MapBlocks blocks[] = {AIR, WATER, LAVA, WALL, FLOOR, START, FINISH, GEM};

void E_SetButtons(TextInfo * buttons, ConfigStruct * GameConfig) {
	// BUTTONCOUNT is defined above to be a constant.
	// [BUTTONCOUNT] number of char pointer pointers pointing to pointers of the
	// addresses of the first chars of my char arrays...
	// because C, and because genuine strings are for weaklings (:
	char * buttonNames[BUTTONCOUNT] = {"SAVE",
						   			   "DELETE",
						   			   "WATER",
						   	  		   "LAVA",
						   			   "WALL",
						   			   "FLOOR",
						   			   "START",
						   			   "FINISH",
						   			   "GEM"};

	// The horizontal and vertical grid positions for the buttons
	int hGrid = GameConfig->WindowWidth * GameConfig->BlockPixel  /  (2 * BUTTONCOUNT - 1);
	int vHight = GameConfig->WindowHeight * GameConfig->BlockPixel - (FONT_XSMALL / 3 * 2);

	for (int i = 0; i < BUTTONCOUNT; i++) {
		buttons[i] = FillTextInfo(buttonNames[i], (2 * i + 1) * hGrid,
								  vHight, FONT_XSMALL, TEXTBOX_CENTERED, GameConfig);
	}

	return;
}

// Function displaying all the buttons in the editor
void E_DrawButtons(TextInfo * buttons, ConfigStruct * GameConfig) {
	SDL_Color whiteFontColor = {0xFF, 0xFF, 0xFF};
	SDL_Color activeFontColor = {0xFF, 0, 0};

	// Searching for an active button
	for (int i = 0; i < BUTTONCOUNT; i++) {
		if (buttons[i].Attribute == ACTIVE)
			buttons[i].Color = activeFontColor;
		else
			buttons[i].Color = whiteFontColor;
		DrawText(&buttons[i], GameConfig);
	}
	SDL_RenderPresent(GameConfig->gRenderer);

	return;
}


// This function returns with the basepath of the ingame blocks FOR THE EDITOR (!)
char * E_GetBlockTexturePath(MapBlocks block) {
	switch (block) {
		case AIR:    return "./assets/e_grid.png";   break;
		case WATER:  return "./assets/e_water.png";  break;
		case LAVA:   return "./assets/e_lava.png";   break;
		case WALL:   return "./assets/wall.png";     break;
		case FLOOR:  return "./assets/floor.png";    break;
		case START:  return "./assets/e_start.png";  break;
		case FINISH: return "./assets/e_finish.png"; break;
		case GEM:    return "./assets/e_gem.png";    break;
	}
}

// Draws the background based on the currently stored map
// NOTE(matthew) this function is quite process heavy as it:
//     - iterates trought the entire grid
//     - loads live textures
//     - and draws/destroyes them
// in itself. This is manageable as it's only the editor, but this is generally
// really bad practice because runtime performance suffers from all this.
void E_SetBackground(MapInfoStruct * map, ConfigStruct * GameConfig) {
	SDL_Texture * texture;

	for (int i = 0; i < GameConfig->WindowWidth; i++) {
	for (int j = 0; j < GameConfig->WindowHeight; j++) {
			SDL_Rect textrect = CalcTextureBox(i * GameConfig->BlockPixel, j * GameConfig->BlockPixel, GameConfig->BlockPixel, GameConfig->BlockPixel, GameConfig);
			texture = LoadTexture(E_GetBlockTexturePath(map->Blocks[i][j]), GameConfig);
			DrawTexture(texture, &textrect, GameConfig);
			SDL_DestroyTexture(texture);
		}
	}

	SDL_RenderPresent(GameConfig->gRenderer);

	return;
}

// Check the mouse's position on the grid to determine if a block has to be placed
void E_CheckMouseGrid(ConfigStruct * GameConfig, TextInfo * buttons, SDL_Point mouse, MapInfoStruct * map) {
	int gridx, gridy;
	if (mouse.y < GameConfig->WindowHeight * GameConfig->BlockPixel * GameConfig->WindowScale) {
		gridx = (int) mouse.x / (GameConfig->BlockPixel * GameConfig->WindowScale);
		gridy = (int) mouse.y / (GameConfig->BlockPixel * GameConfig->WindowScale);

		for (int i = 0; i < BUTTONCOUNT; i++) {
			if (buttons[i].Attribute == ACTIVE) {
				map->Blocks[gridx][gridy] = blocks[i - 1];
				return;
			}
		}
	}

	return;
}

// The core function of the editor, with the map to be opened on the parameterlist
void OpenEditor(char * filePath, ConfigStruct * GameConfig) {
	if (filePath == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		"Error", "No file specified to edit in the Editor. Terminating.\nFor more information, see the documentation.", NULL);

		return;
	}

	// dynamic memory allocation because the whole map blocks array takes up quite some space
	MapInfoStruct * editedMap = (MapInfoStruct *) malloc(sizeof(MapInfoStruct));

	// fill the map with empty blocks
	for (int i = 0; i < GameConfig->WindowWidth; i++) {
		for (int j = 0; j < GameConfig->WindowHeight; j++) {
			editedMap->Blocks[i][j] = AIR;
		}
	}

	// load the map if possible
	SaveLoadMap(filePath, editedMap, LOAD);

	// The struct that stores all the information about buttons
	TextInfo buttons[BUTTONCOUNT];

	// Add an extra row for buttons on the bottom
	GameConfig->WindowHeight += 1;
	// also rename the window
	UpdateWindow("Temple Golem - editor", GameConfig);
	E_SetButtons(buttons, GameConfig);
	// Setting it back to only cover the grid
	GameConfig->WindowHeight -= 1;

	E_SetBackground(editedMap, GameConfig);
	E_DrawButtons(buttons, GameConfig);

	// SDL specific variables
	SDL_Event event;
	SDL_Point mousePos;
	bool running = true;
	bool helddown = false;

	while (running) {
		while (SDL_PollEvent(&event)) {
			SDL_RenderClear(GameConfig->gRenderer);
			switch (event.type) {
				case SDL_KEYDOWN:
					// no key handling in the editor
					break;
				case SDL_MOUSEBUTTONDOWN:
					helddown = true;
					break;
				case SDL_MOUSEBUTTONUP:
					helddown = false;
					break;
				case SDL_QUIT:
					running = false;
					break;
			}
			if (helddown) {
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;
				// If a button is being pressed
				if (CheckMouseButtons(mousePos, buttons, BUTTONCOUNT) != -1) {
					if (buttons[0].Attribute == ACTIVE) {
						SaveLoadMap(filePath, editedMap, SAVE);
						buttons[0].Text = "SAVED";
						buttons[0].Attribute = SHOWN;
					}
					else buttons[0].Text = "SAVE";
				}
				else {
					E_CheckMouseGrid(GameConfig, buttons, mousePos, editedMap);
				}
				E_SetBackground(editedMap, GameConfig);
				E_DrawButtons(buttons, GameConfig);
			}
		}
	}

	// As exiting the app, free the memory allocated for the map itself
	free(editedMap);

	return;
}
