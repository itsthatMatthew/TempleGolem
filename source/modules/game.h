/* The header file for the actual game part of the application */

#ifndef GAME_H
#define GAME_H

#include "./../debugmalloc.h"
#include "./gamestructs.h"
#include "./engines/graphics.h"
#include "./engines/physics.h"
#include <SDL2/SDL.h>

Vector2 FindStart(ConfigStruct * GameConfig, MapInfoStruct * map);

void LoadCharacterTextures(SDL_Texture ** texturearray, ConfigStruct * GameConfig);

void LoadAllTextures(SDL_Texture * texturearray[GEM + 1], ConfigStruct * GameConfig);

SDL_Rect FilterDynamicTextures(SDL_Texture ** textures, MapBlocks block, int frame);

void RasterizeTextures(SDL_Texture * mapTexture, SDL_Rect mapRect, MapInfoStruct * map, SDL_Texture ** textures, ConfigStruct * GameConfig, int frame);

int GameLoop(ConfigStruct * GameConfig, char * mappath, GameEvents ** events);

#endif /* GAME_H */
