#include "./../debugmalloc.h"
#include "./gamestructs.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>

// This function sets the default configuration information to the ConfigStruct
// structure passed to it on the parameter list
void SetDefaultConfigInfo(ConfigStruct * GameConfig) {
	printf("Setting up default configuration values...\n");

	GameConfig->WindowScale  = 4;
	GameConfig->BlockPixel   = PIXELS;
	GameConfig->WindowWidth  = WINDOW_WIDTH;	// d: w1280
	GameConfig->WindowHeight = WINDOW_HEIGHT;	// d: h960
	GameConfig->WindowScreentype = SDL_WINDOW_SHOWN;
	strcpy(GameConfig->UserName, "<Player>");

	printf("Done\n");
	printf("Default values are:\n"
		   "WindowScale: %d\n"
	   	   //"BlockPixel: %d\n"
	   	   //"WindowWidth: %d\n"
	   	   //"WindowHeight: %d\n"
	   	   "WindowScreentype: %d\n"
	   	   "UserName: %s\n", GameConfig->WindowScale, /*GameConfig->BlockPixel, GameConfig->WindowWidth, GameConfig->WindowHeight,*/ GameConfig->WindowScreentype, GameConfig->UserName);
	return;
}

// Creates the config file if it didn't exist
void CreateConfigFile(ConfigStruct * GameConfig) {
	printf("Creating the config file at './assets/config.txt' ...\n");
	FILE * saveptr;
	saveptr = fopen("./assets/config.txt", "wt");

	//fwrite(GameConfig, sizeof(&GameConfig), 1, saveptr);

	fprintf(saveptr, "WindowScale:\t%d\n", GameConfig->WindowScale);
	//fprintf(saveptr, "BlockPixel:\t%d\n", GameConfig->BlockPixel);
	//fprintf(saveptr, "WindowWidth:\t%d\n", GameConfig->WindowWidth);
	//fprintf(saveptr, "WindowHeight:\t%d\n", GameConfig->WindowHeight);
	fprintf(saveptr, "WindowScreentype:\t%d\n", GameConfig->WindowScreentype);
	fprintf(saveptr, "UserName:\t%s\n", GameConfig->UserName);

	fclose(saveptr);

	printf("Done\n");
	return;
}

// Loads the config values (if valid) found at *path to *config
// NOTE(matthew): changed the config file from binary to txt for easier access.
int SaveLoadConfig(ConfigStruct * GameConfig, SaveLoad SL) {
	SL == SAVE ? printf("Saving config to './assets/config.txt'...\n") : printf("Loading config from '%s'...\n", "./assets/config.txt");
	// Opening up the config file
	FILE * fileptr;
	fileptr = fopen("./assets/config.txt", "rt+");

	// if the config file does not exist return
	if (fileptr == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
		"Warning", "The config file (config.txt) does not exists at it's permanently designated location (./assets/config.txt).\nAttempting to create the config file...", NULL);
		CreateConfigFile(GameConfig);
		return -1;	// file does not exist at the given path
	}

	// read the records from the binary file
	//fread(GameConfig, sizeof(*GameConfig), 1, fileptr);
	if (SL == SAVE) {
		fprintf(fileptr, "WindowScale:\t%d\n", GameConfig->WindowScale);
		//fprintf(fileptr, "BlockPixel:\t%d\n", GameConfig->BlockPixel);
		//fprintf(fileptr, "WindowWidth:\t%d\n", GameConfig->WindowWidth);
		//fprintf(fileptr, "WindowHeight:\t%d\n", GameConfig->WindowHeight);
		fprintf(fileptr, "WindowScreentype:\t%d\n", GameConfig->WindowScreentype);
		fprintf(fileptr, "UserName:\t%s\n", GameConfig->UserName);
	} else {
		fscanf(fileptr, "WindowScale:\t%d\n", &GameConfig->WindowScale);
		//fscanf(fileptr, "BlockPixel:\t%d\n", &GameConfig->BlockPixel);
		//fscanf(fileptr, "WindowWidth:\t%d\n", &GameConfig->WindowWidth);
		//fscanf(fileptr, "WindowHeight:\t%d\n", &GameConfig->WindowHeight);
		fscanf(fileptr, "WindowScreentype:\t%d\n", &GameConfig->WindowScreentype);
		fscanf(fileptr, "UserName:\t%s\n", &GameConfig->UserName);
		printf("Loaded values are:\n"
			   "WindowScale: %d\n"
		   	   //"BlockPixel: %d\n"
		   	   //"WindowWidth: %d\n"
		   	   //"WindowHeight: %d\n"
		   	   "WindowScreentype: %d\n"
		   	   "UserName: %s\n", GameConfig->WindowScale, /*GameConfig->BlockPixel, GameConfig->WindowWidth, GameConfig->WindowHeight,*/ GameConfig->WindowScreentype, GameConfig->UserName);
	}

	fclose(fileptr);
	SL == SAVE ? printf("Saved\n") : printf("Loaded\n");
	return 0;	// no errors return
}

// For saving and/or loading a map to the MapInfoStruct
int SaveLoadMap(char * path, MapInfoStruct * map, SaveLoad SL) {
	SL == SAVE ? printf("Saving map to '%s'...\n", path) : printf("Loading map from '%s'...\n", path);
	FILE * fileptr = NULL;

	if (SL == LOAD) {
		fileptr = fopen(path, "rb");

		// check if the map exists
		if (fileptr == NULL) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
			"Warning", "The file at the given path does not exists.\nAttempting to create the file...", NULL);
			printf("Attempting to create file at '%s'...\n", path);

			// the map did not exist, create
			fileptr = fopen(path, "wb");

			if (fileptr == NULL) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
				"Error", "The file at the given path could not be created.\nTerminating.", NULL);
				return 1;
			}

			map->MapName = *path;
			fwrite(map, sizeof(*map), 1 , fileptr);
		}
		else {
			// the map did exist
			fread(map, sizeof(*map), 1 , fileptr);
		}
	}
	else if (SL == SAVE) {
		fileptr = fopen(path, "wb");

		if (fileptr == NULL) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Error", "The file at the given path does not exist.\nTerminating.", NULL);
			return 1;
		}

		fwrite(map, sizeof(*map), 1 , fileptr);
	}

	fclose(fileptr);

	SL == SAVE ? printf("Saved\n") : printf("Loaded\n");
	//debugmalloc_dump();
	return 0;
}

GameEvents * CreateGameEvent(Uint32 tick, GameEvent event) {
	GameEvents * newEvent = (GameEvents *) malloc(sizeof(GameEvents));
	newEvent->Tick = tick;
	newEvent->Event = event;
	newEvent->nxtptr = NULL;
	return newEvent;
}

void AddtoGameEventList(Uint32 tick, GameEvent event, GameEvents ** root) {
	// Create the new element
	GameEvents * newEvent = CreateGameEvent(tick, event);

	// Iterate trough the list till the last event is reached, then add one more after it
	if (*root == NULL) {
		*root = newEvent;
	}
	else {
		GameEvents * iterator = *root;
		while (iterator->nxtptr != NULL) iterator = iterator->nxtptr;
		iterator->nxtptr = newEvent;
	}
	return;
}

void FreeGameEventList(GameEvents * root) {
	// While iterating trought the list, free the previous element
	while(root) {
		GameEvents * next = root->nxtptr;
		free(root);
		root = next;
	}
	return;
}
