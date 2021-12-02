/* This header houses the necessary functions for text manipulation. */

#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <stdbool.h>

// Enumarated type to set font sizes
typedef enum FontSizes {FONT_XSMALL = 12,
						FONT_SMALL  = 24,
						FONT_MEDIUM = 36,
						FONT_LARGE  = 42,
						FONT_XLARGE = 48
} FontSizes;

// Returns the necessary data for a TextInfo variable (used to fill them)
TextInfo FillTextInfo(char * text, int x, int y, int fontSize, TextBoxAlign positioning, ConfigStruct * GameConfig);

// Calculates and returns an SDL_Rect for rendering text(boxes)
// also depending on the alignemnt
SDL_Rect CalcTextBox(TextInfo * text);

// General purpose function that draws text based on the struct passed in
int DrawText(TextInfo * text, ConfigStruct * GameConfig);

// Checks if the mouse is inside a given textbox
bool CheckMouseInsideBox(SDL_Point mouse, TextInfo * box);

// Iterates trough an array of buttons to determine which one (if any) has been pressed
int CheckMouseButtons(SDL_Point mouse, TextInfo * buttons, int buttoncount);

// This function gets the buttons' array as parameter, then returns the index of the button that has been pressed, or -2 if the window's closure is requested.
// On no event, -1 is returned.
int MenuLoop(TextInfo * buttons, int buttoncount, int space, int esc);

#endif /* TEXTMANAGER_H */
