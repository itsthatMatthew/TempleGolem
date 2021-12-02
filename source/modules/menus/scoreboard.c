#include "./../../debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scoreboard.h"
#include "./../textmanager.h"

FILE * OpenScores() {
	FILE * fileptr;
	fileptr = fopen("./assets/scores.txt", "rt+");

	if (fileptr == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
		"Warning", "The scoreboard file (scores.txt) does not exists at it's permanently designated location (./assets/scores.txt).\nAttempting to create the scoreboard file...", NULL);
		fileptr = fopen("./assets/scores.txt", "wt");
		if (fileptr == NULL) {
			return;
		}
		fprintf(fileptr, "scr\ttime\tusername\n");
		fprintf(fileptr, "999\t4.500\t<Cheater>\n");
		fprintf(fileptr, "800\t20.000\t<Professional>\n");
		fprintf(fileptr, "600\t25.000\t<Advanced>\n");
		fprintf(fileptr, "400\t40.000\t<Decent>\n");
		fprintf(fileptr, "200\t60.000\t<Rookie>\n");
		fprintf(fileptr, "0\t120.000\t<Noob>\n");
	}

	fseek(fileptr, 19, 0); // First line unecessary

	return fileptr;
}

void UpdateScoreBoard(int score, double time, ConfigStruct * GameConfig) {
	FILE * fileptr = OpenScores();

	ScoreboardElement * prev = NULL;
	ScoreboardElement * root = NULL;

	// Read saved scores
	while (!feof(fileptr)) {
		ScoreboardElement * newElement = (ScoreboardElement *) malloc(sizeof(ScoreboardElement));
		fscanf(fileptr, "%d\t%lf\t%[^\n] ", &newElement->Score, &newElement->Time, newElement->Username);
		newElement->next = NULL;
		if (prev)
			prev->next = newElement;
		prev = newElement;
		if (!root)
			root = newElement;
	}

	ScoreboardElement * newElement = (ScoreboardElement *) malloc(sizeof(ScoreboardElement));
	newElement->Score = score;
	newElement->Time = time;
	strcpy(newElement->Username, GameConfig->UserName);

	// Determine where the new one fits
	ScoreboardElement * mover = root;
	if (mover->Score > score) {
		for (; mover != NULL && mover->next->Score > score; mover = mover->next) {
			//printf("Score: %d\tTime: %.3lf\tName: %s\n", mover->Score, mover->Time, mover->Username);
		}
		// Mover now points to the one after which we should put it in
		newElement->next = mover->next;
		mover->next = newElement;
	}
	else {
		ScoreboardElement * temp = mover;
		root = newElement;
		newElement->next = mover;
	}


	// Save them again
	fseek(fileptr, 0, 0);
	fprintf(fileptr, "scr\ttime\tusername\n");
	for (mover = root; mover != NULL; mover = mover->next) {
		//printf("Score: %d\tTime: %.3lf\tName: %s\n", mover->Score, mover->Time, mover->Username);
		fprintf(fileptr, "%d\t%.3lf\t%s\n", mover->Score, mover->Time, mover->Username);
	}

	// Free the allocated memory
	fclose(fileptr);
	while (root != NULL) {
		mover = root;
		root = root->next;
		free(mover);
	}

	return;
}

ScoreboardElement * LoadTopTenScores() {
	FILE * fileptr = OpenScores();

	if (!fileptr) return;

	ScoreboardElement * prev = NULL;
	ScoreboardElement * root = NULL;
	int read = 0;

	// Read saved scores
	while (!feof(fileptr) && read < 10) {
		ScoreboardElement * newElement = (ScoreboardElement *) malloc(sizeof(ScoreboardElement));
		fscanf(fileptr, "%d\t%lf\t%[^\n] ", &newElement->Score, &newElement->Time, newElement->Username);
		newElement->next = NULL;
		if (prev)
			prev->next = newElement;
		prev = newElement;
		if (!root)
			root = newElement;
	}

	return root;
}

MenuType DisplayMainMenu(ConfigStruct * GameConfig) {
	SDL_RenderClear(GameConfig->gRenderer);

	SDL_Color MenuFontColor = {0xFF, 0xFF, 0xFF};

	TextInfo labels[13];
	char results[11][30];
	ScoreboardElement * topten = LoadTopTenScores();
	ScoreboardElement * root = topten;

	sprintf(results[0], "score   time   username");
	for (int i = 1; i < 11; i++) {
		sprintf(results[i], "%d   %.3lfs   %s", topten->Score, topten->Time, topten->Username);
		topten = topten->next;
	}

	labels[0] = FillTextInfo("* SCOREBOARD *",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 1.5 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_LARGE, TEXTBOX_CENTERED, GameConfig);

	for (int i = 0; i < 11; i++) {
		labels[i+1] = FillTextInfo(results[i],
		GameConfig->WindowWidth * GameConfig->BlockPixel / 2, (2.75 + i/1.4) * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
		FONT_XSMALL, TEXTBOX_CENTERED, GameConfig);
	}

	labels[12] = FillTextInfo("Return to Main Menu",
	GameConfig->WindowWidth * GameConfig->BlockPixel / 2, 11 * GameConfig->WindowHeight * GameConfig->BlockPixel / 12,
	FONT_SMALL, TEXTBOX_CENTERED, GameConfig);

	for(int i = 0; i < 13; i++) {
		labels[i].Color = MenuFontColor;
		DrawText(&labels[i], GameConfig);
	}

	while (root != NULL) {
		topten = root;
		root = root->next;
		free(topten);
	}

	SDL_RenderPresent(GameConfig->gRenderer);

	while(true) {
		int index = MenuLoop(labels, 13, 12, 12);
		switch (index) {
			case 12:
				return O_MAINMENU;
				break;
			case -2:
				return O_EXIT;
				break;
		}
	}

	return;
}
