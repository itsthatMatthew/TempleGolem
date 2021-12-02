/* Header for the setting menu panel */

#ifndef SETTINGS_H
#define SETTINGS_H

// Enumerated type for indexing the settings menu buttons
typedef enum SMButtons {
	SML_HEADER = 0, SML_PLAYERNAME, SMB_PLAYERNAME, SML_RESOLUTNION, SMB_RESOLUTION, SML_FULLSCREEN, SMB_FULLSCREEN, SMB_BACKTOMM
} SMButtons;

void DrawSettingsMenuButtons(TextInfo * buttons, ConfigStruct * GameConfig);

// setting the new playername
int ChangePlayerName(TextInfo * buttons, ConfigStruct * GameConfig);

// Function that opens and handles the settings
MenuType OpenSettings(ConfigStruct * GameConfig);

void ChangeScreenType(ConfigStruct * GameConfig);

void CycleResolution(ConfigStruct * GameConfig);

#endif
