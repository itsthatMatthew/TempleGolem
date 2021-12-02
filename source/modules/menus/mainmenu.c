#include "./../../debugmalloc.h"
#include "./../gamestructs.h"
#include "./../textmanager.h"
#include "./mainmenu.h"
#include <stdbool.h>

void DrawMainMenuButtons(TextInfo * buttons, ConfigStruct * GameConfig) {
	SDL_RenderClear(GameConfig->gRenderer);

	SDL_Color MenuFontColor = {0xFF, 0xFF, 0xFF};

	char STRusername[14 + 16] = "Player name: ";

	buttons[MM_HEADER] = FillTextInfo("* MAIN MENU *",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 1.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_LARGE, TEXTBOX_CENTERED, GameConfig);

	buttons[MM_PLAYERNAME] = FillTextInfo(strcat(STRusername, GameConfig->UserName),
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 3 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_XSMALL, TEXTBOX_CENTERED, GameConfig);

	buttons[MM_STARTGAMEBUTTON] = FillTextInfo("New Game",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_LARGE, TEXTBOX_CENTERED, GameConfig);

	buttons[MM_SCOREBOARDBUTTON] = FillTextInfo("Scoreboard",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 8 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_LARGE, TEXTBOX_CENTERED, GameConfig);

	buttons[MM_SETTINGSBUTTON] = FillTextInfo("Settings",
	GameConfig->WindowWidth * GameConfig->BlockPixel * 1 / 4, 11 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	buttons[MM_EXITBUTTON] = FillTextInfo("Exit",
	GameConfig->WindowWidth * GameConfig->BlockPixel * 3 / 4, 11 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	for(int i = 0; i <= MM_EXITBUTTON; i++) {
		buttons[i].Color = MenuFontColor;
		DrawText(&buttons[i], GameConfig);
	}
	SDL_RenderPresent(GameConfig->gRenderer);
}

// The function that starts up the main menu panel and handles its events
MenuType OpenMainMenu(ConfigStruct * GameConfig) {
	// Since the exitbutton is the last one, its position determines the overall size
	TextInfo buttons[MM_EXITBUTTON + 1];

	DrawMainMenuButtons(buttons, GameConfig);

	bool running = true;
	int pressed;

	while (running) {
		pressed = MenuLoop(buttons, MM_EXITBUTTON + 1, MM_STARTGAMEBUTTON, MM_EXITBUTTON);
		switch (pressed) {
			// No event
			case -1:
			case MM_HEADER:
				continue;
				break;
			// Exit
			case -2:
			case MM_EXITBUTTON:
				running = false;
				break;
			case MM_PLAYERNAME:
			case MM_SETTINGSBUTTON:
				return O_SETTINGS;
				break;
			case MM_SCOREBOARDBUTTON:
				return O_SCOREBOARD;
				break;
			case MM_STARTGAMEBUTTON:
				return O_NEWGAME;
				break;
		}
	}
	return O_EXIT;
}
