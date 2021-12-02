#include "./../debugmalloc.h"
#include "./gamestructs.h"
#include "./engines/graphics.h"
#include "./engines/physics.h"
#include <SDL2/SDL.h>

Vector2 FindStart(ConfigStruct * GameConfig, MapInfoStruct * map) {
	for (int i = 0; i < GameConfig->WindowWidth; i++) {
	for (int j = 0; j < GameConfig->WindowHeight; j++) {
			if (map->Blocks[i][j] == START)
				return CreateV2(i, j);
		}
	}
	return CreateV2(0, 0);
}

void LoadCharacterTextures(SDL_Texture ** texturearray, ConfigStruct * GameConfig) {
	texturearray[0] = LoadTexture("./assets/GolemSloppy.png", GameConfig);
	texturearray[1] = LoadTexture("./assets/GolemWet.png", GameConfig);
	texturearray[2] = LoadTexture("./assets/Golem.png", GameConfig);
	texturearray[3] = LoadTexture("./assets/GolemLavay.png", GameConfig);
	texturearray[4] = LoadTexture("./assets/GolemMagmatic.png", GameConfig);
}

void LoadAllTextures(SDL_Texture * texturearray[GEM + 1], ConfigStruct * GameConfig) {
	texturearray[AIR] = LoadTexture("./assets/BackTile.png", GameConfig);
	texturearray[WATER] = LoadTexture("./assets/Water.png", GameConfig);
	texturearray[LAVA] = LoadTexture("./assets/Lava.png", GameConfig);
	texturearray[WALL] = LoadTexture("./assets/Wall.png", GameConfig);
	texturearray[FLOOR] = LoadTexture("./assets/Floor.png", GameConfig);
	texturearray[START] = LoadTexture("./assets/OpenDoor.png", GameConfig);
	texturearray[FINISH] = LoadTexture("./assets/ClosedDoor.png", GameConfig);
	texturearray[GEM] = LoadTexture("./assets/Gem.png", GameConfig);
	return;
}

SDL_Rect FilterDynamicTextures(SDL_Texture ** textures, MapBlocks block, int frame) {
	SDL_Rect retRect;
	retRect.x = 0;
	retRect.y = 0;
	retRect.w = 16;
	retRect.h = 16;
	switch (block) {
		// 32x32
		case AIR:
			retRect.w = 32;
			retRect.h = 32;
			break;
		// 16x16 x4
		case WATER:
		case LAVA:
			retRect.x = 16 * (frame % 4);
			break;
		// 16x16 x7
		case GEM:
			retRect.x = 16 * (frame % 7);
			break;
	}
	// default stays 16x16
	return retRect;
}

void RasterizeTextures(SDL_Texture * mapTexture, SDL_Rect mapRect, MapInfoStruct * map, SDL_Texture ** textures, ConfigStruct * GameConfig, int frame) {
	SDL_SetRenderTarget(GameConfig->gRenderer, mapTexture);

	// Nested loop to create the background without the dynamic textures
	for (int i = 0; i < GameConfig->WindowWidth; i++) {
	for (int j = 0; j < GameConfig->WindowHeight; j++) {
			SDL_Rect tempTextureBox = CalcTextureBox(i * GameConfig->BlockPixel, j * GameConfig->BlockPixel, GameConfig->BlockPixel, GameConfig->BlockPixel, GameConfig);
			SDL_Rect crop = FilterDynamicTextures(textures, map->Blocks[i][j], frame);
			SDL_RenderCopy(GameConfig->gRenderer, textures[AIR], NULL, &tempTextureBox);
			SDL_RenderCopy(GameConfig->gRenderer, textures[map->Blocks[i][j]], &crop, &tempTextureBox);
		}
	}

	SDL_SetRenderTarget(GameConfig->gRenderer, NULL);

	DrawTexture(mapTexture, &mapRect, GameConfig);
	//SDL_RenderPresent(gRenderer);

	return;
}

int GameLoop(ConfigStruct * GameConfig, char * mappath, GameEvents ** events) {
	// Tickrate of the game (Hz)
	const int tickrate = 64;
	Uint32 startticks = SDL_GetTicks();
	Uint32 prevstep = startticks;
	int gemscollected = 0;

	// Load the map
	MapInfoStruct * map = (MapInfoStruct *) malloc(sizeof(MapInfoStruct));
	if(SaveLoadMap(mappath, map, LOAD) == 1) {
		free(map);
		return -2;
	}

	// Load the necessary textures beforehand
	SDL_Texture * textures[GEM + 1];
	LoadAllTextures(textures, GameConfig);
	SDL_Texture * characterTextures[5];
	LoadCharacterTextures(characterTextures, GameConfig);

	// Create the background
	SDL_Texture * mapTexture = SDL_CreateTexture(GameConfig->gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GameConfig->WindowWidth * GameConfig->BlockPixel * GameConfig->WindowScale, GameConfig->WindowHeight * GameConfig->BlockPixel * GameConfig->WindowScale);
	// And its rectangle necessary for presenting
	SDL_Rect mapRect = CalcTextureBox(0, 0, GameConfig->WindowWidth * GameConfig->BlockPixel, GameConfig->WindowHeight * GameConfig->BlockPixel, GameConfig);

	RasterizeTextures(mapTexture, mapRect, map, textures, GameConfig, 0);

	// Place and set up the character
	CharacterInfo * character = (CharacterInfo *) malloc(sizeof(CharacterInfo));
	character->Position = CreateV2(0, 0);
	character->Velocity = CreateV2(0, 0);
	character->Mass = 1;
	character->CharacterSate = 0;

	// Find the starting position in it
	character->Position = FindStart(GameConfig, map);
	printf("Starting position set to ( %lf , %lf )\n", character->Position.i, character->Position.j);

	// Start actually looping the game
	bool reset = false;
	bool running = true;
	bool firsuserinput = false;
	bool interrupted = false;
	SDL_Event event;

	// Booleans concerning movement
	bool moveright = false;
	bool moveleft = false;
	bool freefall = false;
	bool jump = false;
	bool supermassive = false;

	while (running && !reset) {
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				// Move the character based on input
				case SDL_KEYDOWN:
					if (!firsuserinput) {firsuserinput = true; startticks = SDL_GetTicks();}
					switch (event.key.keysym.sym) {
						case SDLK_UP: if(!freefall) {jump = true; freefall = true;} break;
						case SDLK_RIGHT: moveright = true; break;
						case SDLK_DOWN: supermassive = true; break;
						case SDLK_LEFT: moveleft = true; break;
						case SDLK_ESCAPE: running = false; interrupted = true; break;
						case SDLK_SPACE: reset = true; break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_RIGHT: moveright = false; break;
						case SDLK_DOWN: supermassive = false; break;
						case SDLK_LEFT: moveleft = false; break;
					}
					break;
				case SDL_QUIT:
					// Close down everything for a proper shutdown
					free(character);
					for (int i = 0; i < GEM; i++) {
						SDL_DestroyTexture(textures[i]);
					}
					for (int i = 0; i < 5; i++) {
						SDL_DestroyTexture(characterTextures[i]);
					}
					SDL_DestroyTexture(mapTexture);
					free(map);
					return -1;
					break;
			}
		}

		Vector2 actingforce = CreateV2(0,0);
		// User inputs
		if (moveright) actingforce = AddV2(actingforce, CreateV2(1.0/tickrate * 128, 0));
		if (moveleft) actingforce = AddV2(actingforce, CreateV2(-1.0/tickrate * 128, 0));
		if (jump) actingforce = AddV2(actingforce, CreateV2(0, -1.0/tickrate * 15360));
		if (supermassive) actingforce = AddV2(actingforce, CreateV2(0, 1.0/tickrate * 512));
		jump = false;
		// Slow the character if the state is altered
		if (fabs(character->CharacterSate) > 1) {
			if (fabs(character->CharacterSate) > 2) {
				if (fabs(character->CharacterSate) > 3) {
					running = false;
					AddtoGameEventList(SDL_GetTicks()-startticks, EVENT_DEATH, events);
				}
				actingforce.j = actingforce.j / 3 * 2;
				actingforce.i = actingforce.i / 4 * 3;
			}
			actingforce.j = actingforce.j / 4 * 3;
		}
		// Natural forces
		actingforce = AddV2(actingforce, CreateV2(0, 1.0/tickrate * 128));
		if (character->Velocity.i < 0) actingforce = AddV2(actingforce, CreateV2(-1.0/tickrate * 64 * character->Velocity.i, 0));
		else if (character->Velocity.i > 0) actingforce = AddV2(actingforce, CreateV2(-1.0/tickrate * 64 * character->Velocity.i, 0));

		// Check for collisions and handle them
		// Load events to a linked list
		MapBlocks touchedBlock = CalculatePhysicsofCharacter(GameConfig, map, character, actingforce, 1.0/tickrate);

		if (touchedBlock == GEM) AddtoGameEventList(SDL_GetTicks()-startticks, EVENT_GEMCOLLECTED, events);
		if (touchedBlock == FINISH) {
			running = false;
			AddtoGameEventList(SDL_GetTicks()-startticks, EVENT_GAMEFINISH, events);
		}
		if (touchedBlock == FLOOR || touchedBlock == WATER || touchedBlock == LAVA) freefall = false;

		// Display and log position
		printf("P(%2.4lf, %2.4lf)\tV(%3.4lf, %3.4lf)\tF(%3.4lf, %3.4lf)\tstate:%1.2lf\ttime:%.3lfs\tticks:%d\r", character->Position.i, character->Position.j, character->Velocity.i, character->Velocity.j, actingforce.i, actingforce.j, character->CharacterSate, firsuserinput ? (SDL_GetTicks()-startticks) / 1000.0 : 0, firsuserinput ? (SDL_GetTicks()-startticks) * tickrate / 1000 : 0);
		RasterizeTextures(mapTexture, mapRect, map, textures, GameConfig, (SDL_GetTicks()-startticks) / 100 );
		DrawCharacter(character, GameConfig, characterTextures);
		SDL_RenderPresent(GameConfig->gRenderer);
		SDL_Delay(SDL_GetTicks()/tickrate - prevstep/tickrate);
		prevstep = SDL_GetTicks();
	}

	// Return and close stuff
	printf("\nExiting game\n");
	free(character);
	for (int i = 0; i < GEM; i++) {
		SDL_DestroyTexture(textures[i]);
	}
	for (int i = 0; i < 5; i++) {
		SDL_DestroyTexture(characterTextures[i]);
	}
	SDL_DestroyTexture(mapTexture);
	free(map);
	if (reset) return 1;
	if (interrupted) return 2;
	return 0;
}
