# Sourcefiles to compile
OBJS = .\main.c .\modules\editor.c .\modules\gamestructs.c .\modules\textmanager.c .\modules\menus\menuloop.c .\modules\menus\mainmenu.c .\modules\menus\settings.c .\modules\menus\scoreboard.c .\modules\menus\newgame.c .\modules\game.c .\modules\engines\graphics.c .\modules\engines\physics.c

# Output file
OBJO = ..\TempleGolem.exe

# C Compiler
CC = gcc -std=c99

# Includes (header files)
INCLUDE_PATHS = -IC:\mingw_dev_lib\include

# Libraries
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

# additional Compilation options
# -w						to supress all warnings
# -Wl,-subsystem,windows	to get rid of the console window
COMPILER_FLAGS = -w #-Wl,-subsystem,windows

# libraries to Link against
LINKER_FLAGS = -lmingw32 -lSDL2_ttf -lSDL2_image -lSDL2main -lSDL2

all : $(OBJS)
	$(CC) $(OBJS) -o $(OBJO) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)
