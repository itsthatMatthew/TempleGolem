/* Header file to manage and calculate the necessary physics related stuff. */

#ifndef PHYSICS_H
#define PHYSICS_H

/* STRUCTS */

typedef enum CharacterStates { SLOPPY = -2 , WET = -1 , NORMAL = 0 , LAVAY = 1 , MAGMATIC = 2 } CharacterStates;

typedef struct Vector2 {
	double i, j;
} Vector2;

typedef struct CharacterInfo {
	Vector2 Position;
	Vector2 Velocity;	// Maximum velocity: 1 unit / tick
	//Vector2 Force;
	double Mass;
	double CharacterSate;
} CharacterInfo;

/* FUNCTIONS */

Vector2 CreateV2(double i, double j);

Vector2 AddV2(Vector2 V, Vector2 W);

/*double ScalarV2(Vector2 V, Vector2 W);

double LengthV2(Vector2 V);

double AngleV2(Vector2 V, Vector2 W);*/

//void CheckBlockCollision(CharacterInfo * character, Vector2 C, Vector2 offset, MapInfoStruct * map);

//void HandleCharacterCollision(CharacterInfo * character, MapInfoStruct * map, ConfigStruct * GameConfig);

MapBlocks CalculatePhysicsofCharacter(ConfigStruct * GameConfig, MapInfoStruct * map, CharacterInfo * character, Vector2 force, double dt);

#endif /* PHYSICS_H */
