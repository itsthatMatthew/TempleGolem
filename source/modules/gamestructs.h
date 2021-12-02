/* This header file contains the type definitions for the game struct. */

#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Declaring the configuration variables
#define WINDOW_WIDTH 20
#define WINDOW_HEIGHT 15
#define PIXELS 16

// This structure stores the cofiguration variables
typedef struct ConfigStruct {
	int WindowScale,	// The scaling of the window (necessary for textures)
		BlockPixel,
		WindowWidth,
		WindowHeight;	// The displaying window's dimensions
	int WindowScreentype;
	TTF_Font * Font;
	char UserName[16 + 1]; // A maximum of 16 cahrs + \0
	SDL_Renderer * gRenderer;
	SDL_Window * gWindow;
} ConfigStruct;

// This enumerated type defines all the possible block types
typedef enum MapBlocks {
	AIR = 0, WATER, LAVA, WALL, FLOOR, START, FINISH, GEM
} MapBlocks;

// This structure stores map information
typedef struct MapInfoStruct {
	char * MapName;	// The name of the map
	MapBlocks Blocks[WINDOW_WIDTH][WINDOW_HEIGHT]; // Zhe blocks the map consists of
	int TotalGemNum; // The total number of gems
} MapInfoStruct;

// This enumerated type defines the alignment of textboxes
typedef enum TextBoxAlign {	TEXTBOX_CENTERED, TEXTBOX_TOPLEFT } TextBoxAlign;
// This enumerated type defines the possible state of buttons
typedef enum ButtonAttribute {SHOWN, HOVER, ACTIVE} ButtonAttribute;

// Enumerated type to determine which menu panel must be shown
typedef enum MenuType {
	O_MAINMENU, O_NEWGAME, O_SCOREBOARD, O_SETTINGS, O_EXIT
} MenuType;

// This struct stores text data used for buttons and any text presentation
typedef struct TextInfo {
	char * Text;
	int X, Y, FontSize;
	SDL_Color Color;
	TextBoxAlign Positioning;
	ButtonAttribute Attribute;
} TextInfo;

// Enumerated type to store gaem events
typedef enum GameEvent {
	EVENT_GEMCOLLECTED, EVENT_SHAPESHIFT, EVENT_DEATH, EVENT_GAMEFINISH
} GameEvent;

// Struct that's used to create a linked list managing game events for final conclusion
typedef struct GameEvents {
	Uint32 Tick;				// when
	GameEvent Event;			// and what happened
	struct GameEvents * nxtptr;	// only the next pointer as we only want to iterate trough it from beginning to end
} GameEvents;

// This function sets the default configuration information to the ConfigStruct
// structure passed to it on the parameter list
void SetDefaultConfigInfo(ConfigStruct * GameConfig);

// Creates the config file if it didn't exist
void CreateConfigFile(ConfigStruct * GameConfig);

// Enumarated value to set the SaveLoadConfig function either one
typedef enum SaveLoad { SAVE, LOAD } SaveLoad;

// Loads the config values (if valid) found at *path to *config
int SaveLoadConfig(ConfigStruct * GameConfig, SaveLoad SL);

// For saving and/or loading a map to the MapInfoStruct
int SaveLoadMap(char * path, MapInfoStruct * map, SaveLoad SL);

void AddtoGameEventList(Uint32 tick, GameEvent event, GameEvents ** root);

void FreeGameEventList(GameEvents * root);

#endif /* GAMESTRUCTS_H */
