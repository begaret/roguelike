#pragma once

#include "terrain.h"
#include "monster.h"
#include "map.h"
#include "data.h"

extern monster_t p;

extern map_t m;

void setup(int load);

void end(void);

void draw(void);

void update(void);

