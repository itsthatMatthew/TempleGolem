#include "./../debugmalloc.h"
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include "./../main.h"
#include "./gamestructs.h"

// Returns the necessary data for a TextInfo variable (used to fill them)
TextInfo FillTextInfo(char * text, int x, int y, int fontSize, TextBoxAlign positioning, ConfigStruct * GameConfig) {
	TextInfo textinfo;

	textinfo.Text = text;
	textinfo.X = x * GameConfig->WindowScale;
	textinfo.Y = y * GameConfig->WindowScale;
	textinfo.FontSize = fontSize * GameConfig->WindowScale;
	textinfo.Positioning = positioning;

	return textinfo;
}

// Calculates and returns an SDL_Rect for rendering text(boxes)
// also depending on the alignemnt
SDL_Rect CalcTextBox(TextInfo * text) {
	int textl = strlen(text->Text);

	SDL_Rect rect;

	// NOTE(matthew): the fonts are 2x as tall as wide
	if (text->Positioning == TEXTBOX_CENTERED) {
		rect.x = text->X - (textl * text->FontSize / 2) / 2;
		rect.y = text->Y - text->FontSize / 2;
		rect.w = textl * text->FontSize / 2;
		rect.h = text->FontSize;
	}
	else if (text->Positioning == TEXTBOX_TOPLEFT) {
		rect.x = text->X;
		rect.y = text->Y;
		rect.w = textl * text->FontSize / 2;
		rect.h = text->FontSize;
	}

	return rect;
}

int DrawText(TextInfo * text, ConfigStruct * GameConfig) {
	SDL_Surface * textsurface = TTF_RenderText_Solid(GameConfig->Font, text->Text, text->Color);
	SDL_Texture * texttexture = SDL_CreateTextureFromSurface(GameConfig->gRenderer, textsurface);

	SDL_Rect textrect = CalcTextBox(text);

	SDL_RenderCopy(GameConfig->gRenderer, texttexture, NULL, &textrect);

	//SDL_RenderPresent(GameConfig->gRenderer);

	SDL_DestroyTexture(texttexture);
	SDL_FreeSurface(textsurface);

	return 0;
}

// Checks if the mouse is inside a given textbox
bool CheckMouseInsideBox(SDL_Point mouse, TextInfo * box) {
	SDL_Rect textbox = CalcTextBox(box);

	if (mouse.x > textbox.x && mouse.x < textbox.x + textbox.w &&
		mouse.y > textbox.y && mouse.y < textbox.y + textbox.h)
		return true;
	return false;
}

// Iterates trough an array of buttons to determine which one (if any) has been pressed
int CheckMouseButtons(SDL_Point mouse, TextInfo * buttons, int buttoncount) {
	int activeindex = -1;

	// check if there is any new button being pressed to begin with
	for (int i = 0; i < buttoncount; i++) {
		if (CheckMouseInsideBox(mouse, &buttons[i]))
			activeindex = i;
	}

	// if there is a new button being pressed, make that the active one
	if (activeindex > -1) {
		for (int i = 0; i < buttoncount; i++) {
			if (i == activeindex)
				buttons[i].Attribute = ACTIVE;
			else
				buttons[i].Attribute = SHOWN;
		}
	}

	// returns -1 if no button was pressed
	return activeindex;
}

// This function gets the buttons' array as parameter, then returns the index of the button that has been pressed, or -2 if the window's closure is requested.
// On no event, -1 is returned.
int MenuLoop(TextInfo * buttons, int buttoncount, int space, int esc) {
	SDL_Event event;
	SDL_Point mousePos;

	bool exit = false;

	while(true) {
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_SPACE)
					return space;
				if(event.key.keysym.sym == SDLK_ESCAPE)
					return esc;
				break;
			case SDL_MOUSEBUTTONUP:
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;
				CheckMouseButtons(mousePos, buttons, buttoncount);
				for (int i = 0; i < buttoncount; i++)
					if (buttons[i].Attribute == ACTIVE)
						return i;
				break;
			case SDL_QUIT:
				return -2;
				break;
		}
		// If no buttonpress is made, simply exit
	}
	}
	// NOTE(matthew) due to rework of the function this part should never be reached
	return -1;
}
