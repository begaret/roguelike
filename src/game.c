#include "game.h"

#include "pause_menu.h"
#include "popup.h"
#include "terminal.h"

#include <string.h>

monster_t p;
map_t m;

static void load_data(void)
{
	mdata_t md[] = {
		{strdup("m_human"), C_WHITE, C_BLACK, '@', 0},
	};

	tdata_t td[] = {
		{strdup("t_grass"),			C_LGREEN,	C_BLACK, ',',	 0},
		{strdup("t_bush"),			C_GREEN,	C_BLACK, '#',	 0},
		{strdup("t_statue"),		C_LGRAY,	C_BLACK, '\xEA', 0},
		{strdup("t_broken_statue"),	C_GRAY,		C_BLACK, '\xEA', 0},
	};

	tclear();

	tcolor(C_WHITE, C_BLACK);
	tborder(0, 0, -1, -1, "loading", NULL);

	tputs(2, 2, "&Cloading monsters");
	for (int i = 0; i < 1; i++) {
		mdata_t data = md[i];

		tcolor(data.color1, data.color2);
		tputc(2 + i, 3, data.tile);
		tflush();
		data_add_mdata(data);
	}

	tputs(2, 5, "&Cloading terrain");
	for (int i = 0; i < 4; i++) {
		tdata_t data = td[i];

		tcolor(data.color1, data.color2);
		tputc(2 + i, 6, data.tile);
		tflush();
		data_add_tdata(data);
	}
}

static void load_save(void)
{
}

static void make_save(void)
{
	// create player
	// create map
	
	generate_map(&m);
	
	p.id = "m_human";
	p.x = 0;
	p.y = 0;
}

void setup(int load)
{
	data_init();
	load_data();

	load ? load_save() : make_save();

	while (1) {
		update();
		draw();	

		int k = tgetc();
		switch (k) {
			case 'y':	p.y--;	p.x--;	break;
			case 'u':	p.y--;	p.x++;	break;
			case 'b':	p.y++;	p.x--;	break;
			case 'n':	p.y++;	p.x++;	break;
			case 'h':	p.x--;	break;
			case 'l':	p.x++;	break;
			case 'k':	p.y--;	break;
			case 'j':	p.y++;	break;
			case K_ESCAPE: {
				if (pause_menu() == 'q') {
					goto end;
				}
			} break; 
		}	
	}

end:
	end();
}

void end(void)
{
	free_map(&m);
	data_exit();
}

void update(void)
{

}

// TODO: make prettier
static void draw_menu(void)
{
	tcolor(C_WHITE, C_BLACK);
	tborder(tx - 24, 0,		 24 - 1,  ty - 1, 	NULL,	NULL);	// status menu
	tborder(0, 		 ty - 8, tx - 25, 7, 		NULL,	NULL);	// message log

	tputs(tx - 22, 2, 	   "&RSTATUS");
	tputs(2,	   ty - 6, "&RMESSAGE");
}

void draw(void)
{
	tclear();

	int w = tx - 24;
	int h = ty - 8;

	// draw map
	int cx = (p.x < w / 2) ? 0 : p.x - w / 2;
	int cy = (p.y < h / 2) ? 0 : p.y - h / 2;
	p.x < MAP_SIZE - w / 2 ?: (cx = MAP_SIZE - w);
	p.y < MAP_SIZE - h / 2 ?: (cy = MAP_SIZE - h);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int mx = x + cx;
			int my = y + cy;
			if (mx < 0 || my < 0
			 || mx >= MAP_SIZE
			 || my >= MAP_SIZE)
				continue;

			tdata_t *data = data_get_tdata(map_get_terrain(&m, mx, my).id);
			tcolor(data->color1, data->color2);
			tputc(x, y, data->tile);	
		}
	}

	mdata_t *pdata = data_get_mdata(p.id);
	tcolor(pdata->color1, pdata->color2);
	tputc(p.x - cx, p.y - cy, pdata->tile);

	draw_menu();

	tflush();
}

