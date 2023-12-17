#include "map.h"

#include "terminal.h"

#include <stdlib.h>

void generate_map(map_t *m)
{
	m->m = NULL;
	
	for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
		m->t[i] = (rand() % 2) ? (terrain_t){"t_grass"} : (terrain_t){"t_bush"};
	}
}

void free_map(map_t *m)
{
	free(m->m);
}

terrain_t map_get_terrain(map_t *m, int x, int y)
{
	if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
		return (terrain_t){0};

	return m->t[x + y * MAP_SIZE];
}

monster_t map_get_monster(map_t *m, int x, int y)
{
	return m && x && y ? (monster_t){0} : (monster_t){0};
}

