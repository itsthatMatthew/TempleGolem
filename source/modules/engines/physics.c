#include "./../../debugmalloc.h"
#include "./../gamestructs.h"
#include <math.h>
#include "physics.h"

/* Underlying physics [fiziksz]
 * F = m * a
 *
 * v(t) = v(0) + a * dt
 * v(t) = v(0) + F / m * dt
 *
 * x(t) = x(0) + v(0) * dt + 1/2 * a * dt^2
 * x(t) = x(0) + v(0) * dt + 1/2 * F / m * dt^2 = x(0) + ( v(t) - 1/2 * F / m * dt ) * dt
 *
 * g = 10 units / 60 dt (= G / m)
 *
 * hmax = v(0)^2 / 2g (= 4 units)
 * v_up = sqrt(2gh) = F_up / m * dt
 * F_up = sqrt(2 * G / m * h) * m / dt = sqrt(2 * G * m * h / dt^2)
 * = sqrt(2gh/dt^2) * m
 */

 // I/O flow:
 // button press -> force calculation -> velocity calculation -> collision detection -> presenting results

#define eps 0.000001

Vector2 CreateV2(double i, double j) {
	Vector2 V;
	V.i = i;
	V.j = j;
	return V;
}

Vector2 AddV2(Vector2 V, Vector2 W) {
	Vector2 R;
	R.i = V.i + W.i;
	R.j = V.j + W.j;
	return R;
}

/*double ScalarV2(Vector2 V, Vector2 W) {
	return V.i * W.i + V.j * W.j;
}

double LengthV2(Vector2 V) {
	return sqrt( (V.i * V.i) + (V.j + V.j) );
}

double AngleV2(Vector2 V, Vector2 W) {
	return acos(ScalarV2(V, W) / (LengthV2(V) * LengthV2(W)));
}*/

MapBlocks CheckBlockCollision(Vector2 C, MapInfoStruct * map) {
	return (map->Blocks[(long) C.i][(long) C.j]);
}

double Fractional(double n) {
	return n - (long) n;
}

void ApplyOffset(Vector2 initpos, Vector2 * offset) {
	Vector2 pos = CreateV2(Fractional(initpos.i), Fractional(initpos.j));
	// Within a block
	// Top right
	if (pos.i >= pos.j) {
		// Top
		if (pos.i + pos.j <= 1)
			offset->j += -1;
		// Right
		else
			offset->i += 1;
	}
	// Bottom left
	else {
		// Left
		if (pos.i + pos.j <= 1)
			offset->i += -1;
		// Bottom
		else
			offset->j += 1;
	}
}

MapBlocks HandleCharacterCollision(CharacterInfo * character, MapInfoStruct * map, ConfigStruct * GameConfig, double dt) {
	Vector2 offset = CreateV2(0,0);
	Vector2 ulPos, urPos, llPos, lrPos;
	ulPos = character->Position;
	urPos = AddV2(character->Position, CreateV2(1-eps,0));
	llPos = AddV2(character->Position, CreateV2(0,1-eps));
	lrPos = AddV2(character->Position, CreateV2(1-eps,1-eps));
	MapBlocks ulBlock, urBlock, llBlock, lrBlock;
	ulBlock = CheckBlockCollision(ulPos, map);
	urBlock = CheckBlockCollision(urPos, map);
	llBlock = CheckBlockCollision(llPos, map);
	lrBlock = CheckBlockCollision(lrPos, map);
	// Left side
	if((ulBlock == WALL || ulBlock == FLOOR) && ((llBlock == WALL || llBlock == FLOOR)))
		offset = AddV2(offset, CreateV2(1,0));
	// Top side
	if((ulBlock == WALL || ulBlock == FLOOR) && (urBlock == WALL || urBlock == FLOOR))
		offset = AddV2(offset, CreateV2(0,1));
	// Right side
	if((urBlock == WALL || urBlock == FLOOR) && (lrBlock == WALL || lrBlock == FLOOR))
		offset = AddV2(offset, CreateV2(-1,0));
	// Bottom side
	if((llBlock == WALL || llBlock == FLOOR) && (lrBlock == WALL || lrBlock == FLOOR))
		offset = AddV2(offset, CreateV2(0,-1));

	// Top reft
	if(ulBlock == WALL || ulBlock == FLOOR)
		ApplyOffset(ulPos, &offset);
	// Top right
	if(urBlock == WALL || urBlock == FLOOR)
		ApplyOffset(urPos , &offset);
	// Bottom left
	if(llBlock == WALL || llBlock == FLOOR)
		ApplyOffset(llPos , &offset);
	// Bottom right
	if(lrBlock == WALL || lrBlock == FLOOR)
		ApplyOffset(lrPos , &offset);


	// Place the character according to the offset
	// Horizontally
	if (offset.i < 0) {
		character->Position.i = (long) character->Position.i;
		character->Velocity.i = 0;
	} else if (offset.i > 0) {
		character->Position.i = (long) character->Position.i + 1;
		character->Velocity.i = 0;
	}
	// Vertically
	if (offset.j < 0) {
		character->Position.j = (long) character->Position.j;
		character->Velocity.j = 0;
	} else if (offset.j > 0) {
		character->Position.j = (long) character->Position.j + 1;
		character->Velocity.j = 0;
	}

	// Handle if the character is outside of the map...
	// Horizontaly
	if (character->Position.i < 0) {
		character->Position = CreateV2(0, character->Position.j);
		character->Velocity = CreateV2(0, character->Velocity.j);
	} else if (character->Position.i > GameConfig->WindowWidth-1){
		character->Position = CreateV2(GameConfig->WindowWidth-1, character->Position.j);
		character->Velocity = CreateV2(0, character->Velocity.j);
	}
	// Verticaly
	if (character->Position.j < 0) {
		character->Position = CreateV2(character->Position.i, 0);
		character->Velocity = CreateV2(character->Velocity.i ,0);
	} else if (character->Position.j > GameConfig->WindowHeight-1){
		character->Position = CreateV2(character->Position.i, GameConfig->WindowHeight-1);
		character->Velocity = CreateV2(character->Velocity.i,0);
		llBlock = FLOOR;
	}

	// Handle if any corner is in Lava/Water
	if (ulBlock == WATER) character->CharacterSate += -0.01;
	if (urBlock == WATER) character->CharacterSate += -0.01;
	if (llBlock == WATER) character->CharacterSate += -0.01;
	if (lrBlock == WATER) character->CharacterSate += -0.01;

	if (ulBlock == LAVA) character->CharacterSate += 0.01;
	if (urBlock == LAVA) character->CharacterSate += 0.01;
	if (llBlock == LAVA) character->CharacterSate += 0.01;
	if (lrBlock == LAVA) character->CharacterSate += 0.01;

	// Determine return block with hierarchy
	// Gem > Finish > Solids
	MapBlocks retBlock = AIR;
	if (ulBlock == GEM || urBlock == GEM || llBlock == GEM || lrBlock == GEM) {
		if (ulBlock == GEM)
			map->Blocks[(long) ulPos.i][(long) ulPos.j] = AIR;
		else if (urBlock == GEM)
			map->Blocks[(long) urPos.i][(long) urPos.j] = AIR;
		else if (llBlock == GEM)
			map->Blocks[(long) llPos.i][(long) llPos.j] = AIR;
		else if (lrBlock == GEM)
			map->Blocks[(long) lrPos.i][(long) lrPos.j] = AIR;

		return GEM;
	}
	else if (ulBlock == FINISH || urBlock == FINISH || llBlock == FINISH || lrBlock == FINISH)
		return FINISH;
	else if (llBlock == FLOOR || lrBlock == FLOOR)
		return FLOOR;

	return retBlock;
}

// Returns true on collision
MapBlocks CalculatePhysicsofCharacter(ConfigStruct * GameConfig, MapInfoStruct * map, CharacterInfo * character, Vector2 force, double dt) {
	//gravity = CreateV2(0, -1 * dt);
	character->Position.i += character->Velocity.i * dt
						   + 1/2 * force.i / character->Mass * dt * dt;

	character->Velocity.i += force.i / character->Mass * dt;
	character->Velocity.j += force.j / character->Mass * dt;

	character->Position.i += character->Velocity.i * dt;
	character->Position.j += character->Velocity.j * dt;

	return HandleCharacterCollision(character, map, GameConfig, dt);
}
