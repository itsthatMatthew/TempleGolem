# Specification for "Temple Golem"

Temple Golem is a simple 2D platformer game.

## The gameplay

In game, the player is moved with the following keys:
- ↑ Up arrow, making the character jump;
- ← Left arrow, making the character move left;
- → Right arrow, making the character move right;
- ↓ Down arrow, making the character fall faster.

In the game, the player tries to clear the map collecting gems at the
fastest speed possible. The end score is calculated accordingly:

[end score] := 1000 / [time in milliseconds]  * ([number of collected gems]^2 + 1) * 1000

This means that collecting no gems still result in a valid run, but dying
automatically results in a score of 0.

### Physics

The character in game is controllable with all 4 of the arrow keys, as specified
above. The character can collide with walls and floor tiles. Only the later will
reset the character's jumping ability. Gems result in no collision, but one
still must collide with them in order to pick them up.

Water and lava have a different effect though. They alter the character's look
and properties, and slow it down all together. Walking in water or lava for a
certain amount of time changes the character's properties. These both apply to
looks and the physical ones as well (such as vertical or horizontal speed).

### Maps

Maps are constructed of 15 (height) x 20 (width) blocks which define the
available window resolutions as well.

## Menus

The menu architecture of the application is quite straight forward. The app
opens at the main menu. From here the settings, the scoreboard or the new game
menu can be reached. In the settings one can change the config settings of the
application, detailed below. On the scoreboard the top 10 scores with all their
details can be viewed. In the new game menu, a new game can be initiated, after
which one can see their results.

### Settings

In the settings one can change:
- the username;
- the resolution (from preset values);
- choose from windowed/fullscreen mode.
