#include "palette.h"

#include "options.h"
#include "config.h"

#include <stdlib.h>

static unsigned palette[0x10];

void palette_init(void)
{
	FILE *fp = fopen(options_get("palette"), "r");
	if (fp == NULL) {
		return;
	}

	config_t cfg = {0};
	config_load(&cfg, fp);

	palette[C_BLACK		] = strtol(config_get(&cfg, "c_black"),		NULL, 0) | 0xFF000000;
	palette[C_BLUE		] = strtol(config_get(&cfg, "c_blue"),		NULL, 0) | 0xFF000000;
	palette[C_GREEN		] = strtol(config_get(&cfg, "c_green"), 	NULL, 0) | 0xFF000000;
	palette[C_CYAN		] = strtol(config_get(&cfg, "c_cyan"), 		NULL, 0) | 0xFF000000;
	palette[C_RED		] = strtol(config_get(&cfg, "c_red"), 		NULL, 0) | 0xFF000000;
	palette[C_MAGENTA	] = strtol(config_get(&cfg, "c_magenta"), 	NULL, 0) | 0xFF000000;
	palette[C_BROWN		] = strtol(config_get(&cfg, "c_brown"), 	NULL, 0) | 0xFF000000;
	palette[C_LGRAY		] = strtol(config_get(&cfg, "c_lgray"), 	NULL, 0) | 0xFF000000;
	palette[C_GRAY		] = strtol(config_get(&cfg, "c_gray"), 		NULL, 0) | 0xFF000000;
	palette[C_LBLUE		] = strtol(config_get(&cfg, "c_lblue"), 	NULL, 0) | 0xFF000000;
	palette[C_LGREEN	] = strtol(config_get(&cfg, "c_lgreen"), 	NULL, 0) | 0xFF000000;
	palette[C_LCYAN		] = strtol(config_get(&cfg, "c_lcyan"), 	NULL, 0) | 0xFF000000;
	palette[C_LRED		] = strtol(config_get(&cfg, "c_lred"), 		NULL, 0) | 0xFF000000;
	palette[C_LMAGENTA	] = strtol(config_get(&cfg, "c_lmagenta"), 	NULL, 0) | 0xFF000000;
	palette[C_YELLOW	] = strtol(config_get(&cfg, "c_yellow"), 	NULL, 0) | 0xFF000000;
	palette[C_WHITE		] = strtol(config_get(&cfg, "c_white"), 	NULL, 0) | 0xFF000000;

	config_free(&cfg);
	fclose(fp);
}

unsigned palette_get(color_t color)
{
	if (color >= 0x10) {
		return 0;
	}

	return palette[color];
}

