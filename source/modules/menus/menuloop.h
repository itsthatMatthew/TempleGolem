/* This header houses a function that drives the menus' events */

#ifndef MENULOOP_H
#define MENULOOP_H

// The core of the menu loop, opening the main menu by default, determining which menu should be the next one to display, up until an indicated app exit, when the function simply returns
void MenuCore(ConfigStruct * GameConfig);

#endif /* MENULOOP_H */
