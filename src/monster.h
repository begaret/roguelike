#pragma once

typedef struct monster_t monster_t;
struct monster_t
{
	int color1 : 4;
	int color2 : 4;
	char tile;
	unsigned flags;
};

