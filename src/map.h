#pragma once

#include "monster.h"
#include "terrain.h"

#define MAP_SIZE 128

typedef struct map_t map_t;
struct map_t
{
	terrain_t t[MAP_SIZE * MAP_SIZE];
	monster_t *m;
};

void generate_map(map_t *m);

void free_map(map_t *m);

terrain_t map_get_terrain(map_t *m, int x, int y);

monster_t map_get_monster(map_t *m, int x, int y);

