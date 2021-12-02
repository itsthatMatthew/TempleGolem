/* Header file for the main menu rendering and handling */

#ifndef MAINMENU_H
#define MAINMENU_H

// Enumerated type for indexing the main menu buttons
typedef enum MMButtons {MM_HEADER = 0, MM_PLAYERNAME, MM_STARTGAMEBUTTON, MM_SCOREBOARDBUTTON, MM_SETTINGSBUTTON, MM_EXITBUTTON} MMButtons;

void DrawMainMenuButtons(TextInfo * buttons, ConfigStruct * GameConfig);

// The function that starts up the main menu panel and handles its events
MenuType OpenMainMenu(ConfigStruct * GameConfig);

#endif /* MAIN_MENU */
