#include "./../../debugmalloc.h"
#include <stdbool.h>
#include "./../gamestructs.h"
#include "./../textmanager.h"
#include "./mainmenu.h"
#include "./settings.h"
#include "./newgame.h"
#include "./menuloop.h"
#include "./../game.h"

/* ===== THE LOOPING CORE =====*/

// The core of the menu loop, opening the main menu by default, determining which menu should be the next one to display, up until an indicated app exit, when the function simply returns
void MenuCore(ConfigStruct * GameConfig) {
	MenuType nextmenu = OpenMainMenu(GameConfig);
	while (nextmenu != O_EXIT) {
		switch (nextmenu) {
			case O_EXIT:
				break;
			case O_MAINMENU:
				nextmenu = OpenMainMenu(GameConfig);
				break;
			case O_SCOREBOARD:
				nextmenu = DisplayMainMenu(GameConfig);
				break;
			case O_SETTINGS:
				nextmenu = OpenSettings(GameConfig);
				break;
			case O_NEWGAME:
				nextmenu = OpenNewGameMenu(GameConfig);
				break;
		}
	}

	return;
}
