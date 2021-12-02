/* The base point of the source code. */
/* NOTE(matthew): troughout the project Lazy Foo's SDL tutorial acted as a
 * baseline and inspiration for code and data structures. Many of their work
 * can be found or traced in my code, so, credit is done where credit is due. */

/* ===== INCLUDES ===== */

#include "./debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./modules/editor.h"
#include "./modules/gamestructs.h"
#include "./modules/menus/menuloop.h"
#include "./modules/textmanager.h"
#include "./modules/engines/graphics.h"
#include "./modules/engines/physics.h"

/* ===== MAIN ===== */

int main(int argc, char * argv[]) {
	printf("Starting 'temple Golem' v0.4.35 [alpha] (wip)\n\n");

	// Struct that stores program specific information, such as the dimensions of the window
	ConfigStruct * GameInfo = (ConfigStruct *) malloc(sizeof(ConfigStruct));

	// Setting the config variables
	SetDefaultConfigInfo(GameInfo);
	// And loading saved config if possible
	SaveLoadConfig(GameInfo, LOAD);

	// SDL setup
	// If unsuccesful, terminate
	if (SetupSDL("Temple Golem", GameInfo) != 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		"Error", "SDL initialization was unsuccesful.\nTerminating", NULL);
		return 1;
	}

	// If there are no program arguments
	if (argc == 1) {
		// Driving the program based on the input events
		MenuCore(GameInfo);
	}
	// If the second program argument is to open the editor
	else if (strcmp(argv[1], "-e") == 0) {
		// Editor mode to manipulate a map
		OpenEditor(argv[2], GameInfo);
	}
	else {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		"Error", "Incorrect input program arguments.\nTerminating", NULL);
	}

	// Closing down SDL
	CloseSDL(GameInfo);

	free(GameInfo);

	// Checking for memory leaks
	//debugmalloc_dump(); //  Not necessary

	return 0;
}
