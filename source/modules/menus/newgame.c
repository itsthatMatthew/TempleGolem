#include "./../../debugmalloc.h"
#include "./../gamestructs.h"
#include "./../textmanager.h"
#include "./newgame.h"
#include "./scoreboard.h"
#include <stdbool.h>

void DrawNewGameMenuButtons(TextInfo * buttons, ConfigStruct * GameConfig, int score, double finaltime) {
	SDL_RenderClear(GameConfig->gRenderer);

	SDL_Color MenuFontColor = {0xFF, 0xFF, 0xFF};

	buttons[NGM_HEADER] = FillTextInfo("* NEW GAME *",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 1.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_LARGE, TEXTBOX_CENTERED, GameConfig);

	buttons[PLAY_DEFAULT_MAP] = FillTextInfo("Play the Default Map",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	char scorelabel[12];
	sprintf(scorelabel, "score: %d", score);
	buttons[SCORE_LABEL] = FillTextInfo(scorelabel,
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 7 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	char timelabel[15];
	sprintf(timelabel, "time: %.3lfs", finaltime);
	buttons[FINAL_TIME_LABEL] = FillTextInfo(timelabel,
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 9 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	buttons[NGM_BACKTOMM] = FillTextInfo("Return to Main Menu",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 11 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	for(int i = 0; i <= NGM_BACKTOMM; i++) {
		buttons[i].Color = MenuFontColor;
		DrawText(&buttons[i], GameConfig);
	}
	SDL_RenderPresent(GameConfig->gRenderer);

	return;
}

// Setting the cunstom map's reach path
/*int SetCustomMapPath(char * mappath, TextInfo * buttons, ConfigStruct * GameConfig) {
	int charcount = 0;
	bool active = true;
	bool refresh = true;
	SDL_Event event;

	//buttons[CUSTOM_MAP_LABEL].Attribute = ACTIVE;

	SDL_StartTextInput();

	while (active) {
		//active = false;
		while(SDL_PollEvent(&event)) {
			if (charcount == 99)
			switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_BACKSPACE && charcount > 0)
						mappath[--charcount] = '\0';
					else if (event.key.keysym.sym == SDLK_RETURN)
                            active = false;
					refresh = true;
					break;
 				case SDL_TEXTINPUT:
                    strcat(mappath, event.text.text);
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
				DrawNewGameMenuButtons(buttons, mappath, GameConfig);
				refresh = false;
			}
		}
	}

	SDL_StopTextInput();
	return 0;
}*/

MenuType OpenNewGameMenu(ConfigStruct * GameConfig) {
	TextInfo buttons[NGM_BACKTOMM + 1];
	char mappath[100] = "<wip>";

	//DrawNewGameMenuButtons(buttons, mappath);

	bool running = true;
	int flag = 1;

	GameEvents * events = NULL;

	DrawNewGameMenuButtons(buttons, GameConfig, 0, 0);

	while (running) {
		switch (MenuLoop(buttons, NGM_BACKTOMM + 1, PLAY_DEFAULT_MAP, NGM_BACKTOMM)) {
			// Exit app
			case -2:
				return O_EXIT;
				break;
			// Exit setting
			case NGM_BACKTOMM:
				running = false;
				break;
			// Play the default map
			case PLAY_DEFAULT_MAP:
				flag = 1;
				events = NULL;
					while (flag == 1) {
						FreeGameEventList(events);
						events = NULL;
						flag = GameLoop(GameConfig, "./assets/TempleRun.tgm", &events);
					}
					//Calculating score
					int gems = 0, endtime = 1, multiplier = 1;
					for (GameEvents * iterator = events; iterator; iterator = iterator->nxtptr) {
						printf("Game event: t%d ", iterator->Tick);
						switch (iterator->Event) {
							case EVENT_GEMCOLLECTED:
								gems++;
								printf("EVENT_GEMCOLLECTED\n");
								break;
							case EVENT_GAMEFINISH:
								endtime = iterator->Tick;
								printf("EVENT_GAMEFINISH\n");
								break;
							case EVENT_DEATH:
								multiplier = 0;
								endtime = iterator->Tick;
								printf("EVENT_DEATH\n");
								break;
						}
					}
					int score = 1000.0/endtime * (gems * gems + 1) * 1000 * multiplier;
					printf("Final score: %d\nFinished the level in %g seconds\n", score, endtime/1000.0);
					if (flag != -1 && flag != 2) UpdateScoreBoard(score, endtime/1000.0, GameConfig);
					FreeGameEventList(events);
					events = NULL;
					if(flag == -1)
						return O_EXIT;
					DrawNewGameMenuButtons(buttons, GameConfig, score, endtime/1000.0);
				break;
			// Play a custom map
			/*case PLAY_CUSTOM_MAP:
				//if(GameLoop(&GameInfo,mappath) == -1)
				//	return O_EXIT;
				break;
			// Set a custom map's path
			case CUSTOM_MAP_LABEL:
				//free(mappath);
			 	//if (SetCustomMapPath(mappath, buttons) == -1)
				//	return O_EXIT;
				break;*/
		}
	}
	return O_MAINMENU;
}
