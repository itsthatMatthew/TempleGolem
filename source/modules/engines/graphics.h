/* Header file to manage texture loading and other graphics related stuff. */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "./physics.h"

/* ===== SDL RELATED FUNCTIONS ===== */

// Function to set up SDL
int SetupSDL(char * name, ConfigStruct * GameConfig);

// Function to update the window's attributes
void UpdateWindow(char * name, ConfigStruct * GameConfig);

// Function to close SDL
void CloseSDL(ConfigStruct * GameConfig);

SDL_Color MakeSDLColor(int r, int g, int b);

/* ===== GRAPHICS ENGINE IMPLEMENTATION ===== */

// A slightly modified version of CalcTextBox to calculate a render box for the
// texture
SDL_Rect CalcTextureBox(int x, int y, int w, int h, ConfigStruct * GameConfig);

// This function creates the "missing texture" texture to be displayed if a
// texture is not found at its specified path
SDL_Texture * MissingTexture(ConfigStruct * GameConfig);

// This function loads the file from the specified FilePath to the pointer
SDL_Texture * LoadTexture(char * filePath, ConfigStruct * GameConfig);

// This function draws the texture
void DrawTexture(SDL_Texture * texture, SDL_Rect * texturebox, ConfigStruct * GameConfig);

void DrawCharacter(CharacterInfo * character, ConfigStruct * GameConfig, SDL_Texture ** characterTexture);

#endif /* GRAPHICS_H */
