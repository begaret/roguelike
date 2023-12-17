#include "game.h"

#include "popup.h"
#include "terminal.h"

#include <unistd.h>

monster_t p;
map_t m;

static void load_data(void)
{
	mdata_t md[] = {
		{strdup("m_human"), C_WHITE, C_BLACK, '@', 0}
	};

	tdata_t td[] = {
		{strdup("t_grass"),	C_LGREEN,	C_BLACK, ',', 0},
		{strdup("t_bush"),	C_GREEN,	C_BLACK, '#', 0}
	};

	terminal_clear();

	terminal_color(C_WHITE, C_BLACK);
	terminal_border(0, 0, -1, -1);
	terminal_puts(-1, 0, "\xAE &Yloading&W \xAF");

	terminal_puts(2, 2, "&Cloading monsters");
	for (int i = 0; i < 1; i++) {
		mdata_t data = md[i];

		terminal_color(data.color1, data.color2);
		terminal_putc(2 + i, 3, data.tile);
		terminal_refresh();
		data_add_mdata(data);
	}

	terminal_puts(2, 5, "&Cloading terrain");
	for (int i = 0; i < 2; i++) {
		tdata_t data = td[i];

		terminal_color(data.color1, data.color2);
		terminal_putc(2 + i, 6, data.tile);
		terminal_refresh();
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

	// run();
	while (1) {
		int k = terminal_getc();
		if (k == K_ESCAPE)
			break;
	}
}

void run(void)
{
	int w, h;
	terminal_size(&w, &h);
	while (1) {
		int x1 = p.x - w / 2;
		int y1 = p.y - h / 2;
		int x2 = x1 + w;
		int y2 = y1 + h;

		terminal_clear();
		for (int x = x1; x < x2; x++) {
			for (int y = y1; y < y2; y++) {
				if (x < 0 || x >= MAP_SIZE 
				 || y < 0 || y >= MAP_SIZE)
					continue;

				terrain_t t = map_get_terrain(&m, x, y);
				tdata_t *data = data_get_tdata(t.id);
				if (data) {
					terminal_color(data->color1, data->color2);
					terminal_putc(x - x1, y - y1, data->tile);
				} else {
					terminal_color(C_LGRAY, C_BLACK);
					terminal_putc(x - x1, y - y1, '?');
				}
			}
		}

		mdata_t *data = data_get_mdata(p.id);
		if (data) {
			terminal_color(data->color1, data->color2);
			terminal_putc(w / 2, h / 2, data->tile);
		} else {
			terminal_color(C_LGRAY, C_BLACK);
			terminal_putc(w / 2, h / 2, '?');
		}

		terminal_refresh();

		int k = terminal_getc();
		if (k == K_LEFT) {
			p.x--;
		} else if (k == K_RIGHT) {
			p.x++;
		} else if (k == K_UP) {
			p.y--;
		} else if (k == K_DOWN) {
			p.y++;
		} else if (k == K_ESCAPE) {
			data_exit();
			free_map(&m);	// TODO: move
			return;
		}
	}
}

void update(void)
{

}

void draw(void)
{

}

