#include "./../../debugmalloc.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./../gamestructs.h"
#include "./../textmanager.h"
#include "./physics.h"

/* ===== SDL RELATED FUNCTIONS ===== */

// Function to set up SDL
int SetupSDL(char * name, ConfigStruct * GameConfig) {
	printf("Initializing SDL...\n");
	// Initializing SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1; // Unsuccesful base init.
	printf("SDL_Init OK\n");

	// Creating the window
	GameConfig->gWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GameConfig->WindowWidth * GameConfig->BlockPixel * GameConfig->WindowScale, GameConfig->WindowHeight * GameConfig->BlockPixel * GameConfig->WindowScale, GameConfig->WindowScreentype);
	if (GameConfig->gWindow == NULL)
		return 1; // Unsuccesful base init.
	printf("SDL_CreateWindow OK\n");

	// Creating the renderer
	GameConfig->gRenderer = SDL_CreateRenderer(GameConfig->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (GameConfig->gRenderer == NULL)
		return 1; // Unsuccesful base init.
	SDL_RenderClear(GameConfig->gRenderer);
	printf("SDL_CreateRenderer OK\n");

	// TTF initialization
	if (TTF_Init() == -1)
		return 2; // Unsuccesful ttf init.
	GameConfig->Font = TTF_OpenFont("./assets/RetroBound.ttf", FONT_XLARGE);
	if (GameConfig->Font == NULL)
		return 2;
	printf("TTF_Init OK\n");

	// IMG initialization for PNG image usage
	if (IMG_Init(IMG_INIT_PNG) == 0)
		return 3; // Unsuccesful img init.
	printf("IMG_Init OK\n");

	printf("SDL Init DONE.\n");
	return 0; // Succesful initialization
}

// Function to update the window's attributes
void UpdateWindow(char * name, ConfigStruct * GameConfig) {
	printf("Updating window...\n");
	// Resizing
	SDL_SetWindowSize(GameConfig->gWindow, GameConfig->WindowWidth * GameConfig->BlockPixel * GameConfig->WindowScale, GameConfig->WindowHeight * GameConfig->BlockPixel * GameConfig->WindowScale);
	// Recentering
	SDL_SetWindowPosition(GameConfig->gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	// Fullscreen setting
	SDL_SetWindowFullscreen(GameConfig->gWindow, GameConfig->WindowScreentype);
	// Renaming
	SDL_SetWindowTitle(GameConfig->gWindow, name);
	printf("Done\n");
	return; // Endpoint
}

// Function to close SDL
void CloseSDL(ConfigStruct * GameConfig) {
	printf("Closing SDL...\n");
	// Destroying the window
	SDL_DestroyWindow(GameConfig->gWindow);
	// Setting the pointers back to NULLs
	GameConfig->gWindow = NULL;
	GameConfig->gRenderer = NULL;

	TTF_CloseFont(GameConfig->Font);

	// Quitting SDL subs
	SDL_Quit();
	printf("SDL succesfuly closed.\n");
	return;
}

SDL_Color MakeSDLColor(int r, int g, int b) {
	SDL_Color color = {r, g, b};
	return color;
}

/* ===== GRAPHICS ENGINE IMPLEMENTATION ===== */

// A slightly modified version of CalcTextBox to calculate a render box for the
// texture
SDL_Rect CalcTextureBox(int x, int y, int w, int h, ConfigStruct * GameConfig) {
	SDL_Rect rect;

	rect.x = x * GameConfig->WindowScale;
	rect.y = y * GameConfig->WindowScale;
	rect.w = w * GameConfig->WindowScale;
	rect.h = h * GameConfig->WindowScale;

	return rect;
}

// This function creates the "missing texture" texture to be displayed if a
// texture is not found at its specified path
SDL_Texture * MissingTexture(ConfigStruct * GameConfig) {
	Uint16 notexture[8*8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			notexture[(i + 1) + (j * 8) - 1] = i % 2 ^ j % 2 ? 0xff0f : 0xf000 ;
		}
	}

	SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(notexture, 8, 8, 16, 16, 0x0f00, 0x00f0, 0x000f, 0xf000);
	SDL_Texture * missingtexture = SDL_CreateTextureFromSurface(GameConfig->gRenderer, surface);
	SDL_FreeSurface(surface);
	surface = NULL;

	return missingtexture;
}

// This function loads the file from the specified FilePath to the pointer
SDL_Texture * LoadTexture(char * filePath, ConfigStruct * GameConfig) {
	SDL_Texture * texture = IMG_LoadTexture(GameConfig->gRenderer, filePath);
	if (texture == NULL) {
		printf("Unable to load texture from '%s'\n", filePath);
		return MissingTexture(GameConfig);
	}
	return texture;
}

// This function draws the texture
void DrawTexture(SDL_Texture * texture, SDL_Rect * texturebox, ConfigStruct * GameConfig) {
	SDL_RenderCopy(GameConfig->gRenderer, texture, NULL, texturebox);
	// Destroying the texture is on the caller
	return;
}

SDL_Texture * DrawCorrectCharacterState(CharacterInfo * character, SDL_Texture ** characterTextures) {
	if (character->CharacterSate < -1) {
		if (character->CharacterSate < -2) {
			return characterTextures[0];
		}
		return characterTextures[1];
	}
	if (character->CharacterSate > 1) {
		if (character->CharacterSate > 2) {
			return characterTextures[4];
		}
		return characterTextures[3];
	}
	return characterTextures[2];
}

void DrawCharacter(CharacterInfo * character, ConfigStruct * GameConfig, SDL_Texture ** characterTextures) {
	SDL_Texture * golem = DrawCorrectCharacterState(character, characterTextures);
	SDL_Rect pos = CalcTextureBox(character->Position.i * GameConfig->BlockPixel, character->Position.j * GameConfig->BlockPixel, 16, 16, GameConfig);
	DrawTexture(golem, &pos, GameConfig);
	return;
}
