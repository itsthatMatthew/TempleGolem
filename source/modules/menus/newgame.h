/* The map chosing menu before hte actual gameplay */

#ifndef NEWGAME_H
#define NEWGAME_H

typedef enum NGMButtons {
	NGM_HEADER = 0, PLAY_DEFAULT_MAP, SCORE_LABEL, FINAL_TIME_LABEL, NGM_BACKTOMM
} NGMButtons;

void DrawNewGameMenuButtons(TextInfo * buttons, ConfigStruct * GameConfig, int score, double time);

MenuType OpenNewGameMenu(ConfigStruct * GameConfig);

#endif /* NEWGAME_H */
