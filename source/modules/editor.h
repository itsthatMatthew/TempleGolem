/* The header file of the map editor of the game. */

#ifndef GAMEEDITOR_H
#define GAMEEDITOR_H

#include "./gamestructs.h"
#include "./textmanager.h"

void E_SetButtons(TextInfo * buttons, ConfigStruct * GameConfig);

// Function displaying all the buttons in the editor
void E_DrawButtons(TextInfo * buttons, ConfigStruct * GameConfig);

// This function returns with the basepath of the ingame blocks FOR THE EDITOR (!)
char * E_GetBlockTexturePath(MapBlocks block, ConfigStruct * GameConfig);

// Check the mouse's position on the grid to determine if a block has to be placed
void E_CheckMouseGrid(TextInfo * buttons, SDL_Point mouse, MapInfoStruct * map);

// The core function of the editor, with the map to be opened on the parameterlist
void OpenEditor(char * filePath, ConfigStruct * GameConfig);

#endif /* GAMEEDITOR_H */
