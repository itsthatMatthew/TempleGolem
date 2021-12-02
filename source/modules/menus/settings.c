#include "./../../debugmalloc.h"
#include "./../gamestructs.h"
#include "./../textmanager.h"
#include "./settings.h"
#include <stdbool.h>

void DrawSettingsMenuButtons(TextInfo * buttons, ConfigStruct * GameConfig) {
	SDL_RenderClear(GameConfig->gRenderer);

	SDL_Color MenuFontColor = {0xFF, 0xFF, 0xFF};
	SDL_Color ActiveFontColor = {0xFF, 0x00, 0x00};

	buttons[SML_HEADER] = FillTextInfo("* SETTINGS *",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 1.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_LARGE, TEXTBOX_CENTERED, GameConfig);

	/* PLAYERNAME */

	buttons[SML_PLAYERNAME] = FillTextInfo("Player name:",
	GameConfig->WindowWidth * GameConfig->BlockPixel * 0.5 / 12, 2.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_TOPLEFT, GameConfig);

	buttons[SMB_PLAYERNAME] = FillTextInfo(GameConfig->UserName,
	GameConfig->WindowWidth * GameConfig->BlockPixel * 4 / 12, 3.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_TOPLEFT, GameConfig);

	/* RESOLUTION */

	buttons[SML_RESOLUTNION] = FillTextInfo("Resolution:",
	GameConfig->WindowWidth * GameConfig->BlockPixel * 0.5 / 12, 5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_TOPLEFT, GameConfig);

	char resolutionstring[12];

	sprintf(resolutionstring, "<%dx%dpx>",GameConfig->WindowWidth * GameConfig->BlockPixel * GameConfig->WindowScale, GameConfig->WindowHeight * GameConfig->BlockPixel * GameConfig->WindowScale);

	//NOTE(matthew): for some (...) reason only the first 3 characters register
	buttons[SMB_RESOLUTION] = FillTextInfo(resolutionstring,
	GameConfig->WindowWidth * GameConfig->BlockPixel * 4 / 12, 6 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_TOPLEFT, GameConfig);

	/* FULLSCREEN */

	buttons[SML_FULLSCREEN] = FillTextInfo("Fullscreen:",
	GameConfig->WindowWidth * GameConfig->BlockPixel * 0.5 / 12, 7.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_TOPLEFT, GameConfig);

	buttons[SMB_FULLSCREEN] = FillTextInfo(GameConfig->WindowScreentype == SDL_WINDOW_FULLSCREEN ? "<Fullscreen>" : "<Windowed>",
	GameConfig->WindowWidth * GameConfig->BlockPixel * 4 / 12, 8.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_TOPLEFT, GameConfig);

	/* BACK TO MAIN MENU */

	buttons[SMB_BACKTOMM] = FillTextInfo("Return to Main Menu",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 11 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	for(int i = 0; i <= SMB_BACKTOMM; i++) {
		if (buttons[i].Attribute == ACTIVE)
			buttons[i].Color = ActiveFontColor;
		else
			buttons[i].Color = MenuFontColor;
		DrawText(&buttons[i], GameConfig);
	}
	SDL_RenderPresent(GameConfig->gRenderer);
}

// setting the new playername
int ChangePlayerName(TextInfo * buttons, ConfigStruct * GameConfig) {
	char newusername[16 + 1] = {0};
	strcpy(GameConfig->UserName, newusername);
	DrawSettingsMenuButtons(buttons, GameConfig);
	char charcount = 0;
	bool active = true;
	bool refresh = true;
	SDL_Event event;

	buttons[SMB_PLAYERNAME].Attribute = ACTIVE;

	SDL_StartTextInput();

	while (active) {
		while(SDL_PollEvent(&event)) {

			if (charcount > 15) {
				active = false;
				break;
			}

			switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_BACKSPACE && charcount > 0)
						newusername[--charcount] = 0;
					else if (event.key.keysym.sym == SDLK_RETURN)
                            active = false;
					refresh = true;
					break;
 				case SDL_TEXTINPUT:
                    strcat(newusername, event.text.text);
					charcount++;
					refresh = true;
                    break;
				case SDL_MOUSEBUTTONDOWN:
					active = false;
					refresh = true;
					break;
				case SDL_QUIT:
					return -1;
					break;
				default:
					break;
			}

			if (refresh == true) {
				//printf("Button pressed for input.\n");
				strcpy(GameConfig->UserName, newusername);
				DrawSettingsMenuButtons(buttons, GameConfig);
				refresh = false;
			}

			if (active == false) break;
		}
	}

	SDL_StopTextInput();
	strcpy(GameConfig->UserName, newusername);
	return 0;
}

void ChangeScreenType(ConfigStruct * GameConfig) {
	if (GameConfig->WindowScreentype == SDL_WINDOW_SHOWN)
		GameConfig->WindowScreentype = SDL_WINDOW_FULLSCREEN;
	else
		GameConfig->WindowScreentype = SDL_WINDOW_SHOWN;
	UpdateWindow("TempleGolem", GameConfig);
	return;
}

void CycleResolution(ConfigStruct * GameConfig) {
	switch (GameConfig->WindowScale) {
		case 3:
		case 4:
		case 5:
			GameConfig->WindowScale++;
			break;
		default:
			GameConfig->WindowScale = 3;
			break;
	}
	UpdateWindow("TempleGolem", GameConfig);

	return;
}

// Function that opens and handles the settings
MenuType OpenSettings(ConfigStruct * GameConfig) {
	TextInfo buttons[SMB_BACKTOMM + 1];

	//DrawSettingsMenuButtons(buttons);

	bool running = true;

	while (running) {
		DrawSettingsMenuButtons(buttons, GameConfig);
		switch (MenuLoop(buttons, SMB_BACKTOMM + 1, -1, SMB_BACKTOMM)) {
			// Exit app
			case -2:
				return O_EXIT;
				break;
			// Exit setting
			case SMB_BACKTOMM:
				SaveLoadConfig(GameConfig, SAVE);
				running = false;
				break;
			// Namechanging
			case SML_PLAYERNAME:
			case SMB_PLAYERNAME:
				if (ChangePlayerName(buttons, GameConfig) == -1)
					return O_EXIT;
				break;
			// Changing screen type
			case SML_FULLSCREEN:
			case SMB_FULLSCREEN:
				ChangeScreenType(GameConfig);
				break;
			// Changing screen size
			case SML_RESOLUTNION:
			case SMB_RESOLUTION:
				CycleResolution(GameConfig);
				break;
			// No event
			default:
				break;
		}
	}
	return O_MAINMENU;
}
