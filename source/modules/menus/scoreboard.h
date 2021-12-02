/* The header file to handle and manage the scoreboard and stats. */

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "./../gamestructs.h"

// Noob -200
// Rookie 200-400
// Decent 400-600
// Advanced 600-800
// Professional 800-1000
// Cheater 1000-

typedef struct ScoreboardElement {
	int Score;
	double Time;
	char Username[16 + 1];
	struct ScoreboardElement * next;
} ScoreboardElement;

void UpdateScoreBoard(int score, double time, ConfigStruct * GameConfig);

ScoreboardElement * LoadTopTenScores();

#endif /* SCOREBOARD_H */
