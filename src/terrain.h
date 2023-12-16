#pragma once

typedef struct terrain_t terrain_t;
struct terrain_t
{
	int color1 : 4;
	int color2 : 4;
	char tile;
	unsigned flags;
};

