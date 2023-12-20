#include "options.h"

#include "config.h"

#define DEFAULT_TILESET  "curses.bmp"
#define DEFAULT_PALETTE	 "colors.txt"
#define DEFAULT_WINDOW_X 80
#define DEFAULT_WINDOW_Y 25
#define DEFAULT_WINDOWED 1
#define DEFAULT_DISPLAY	 "opengl"

static config_t options_cfg = {0};

static FILE *create_options(void)
{
	FILE *fp = fopen("options.txt", "w");
	if (fp == NULL) {
		return NULL;
	}

	fprintf(fp, "tileset[%s]\n",	DEFAULT_TILESET);
	fprintf(fp, "palette[%s]\n",	DEFAULT_PALETTE);
	fprintf(fp, "window_x[%i]\n",	DEFAULT_WINDOW_X);
	fprintf(fp, "window_y[%i]\n",	DEFAULT_WINDOW_Y);
	fprintf(fp, "windowed[%i]\n",	DEFAULT_WINDOWED);
	fprintf(fp, "display[%s]\n",	DEFAULT_DISPLAY);

	fclose(fp);

	return fopen("options.txt", "r");
}

void options_init(void)
{
	FILE *fp = fopen("options.txt", "r");
	if (fp == NULL) {
		fp = create_options();
	}

	config_load(&options_cfg, fp);

	fclose(fp);
}

void options_exit(void)
{
	config_free(&options_cfg);
}

char *options_get(char *k)
{
	return config_get(&options_cfg, k);
}

