#include "game.h"

#include "terminal.h"
#include "popup.h"

static void load_player(void)
{
	popup_yn("load character?");	
	char *s = popup_s("enter character name");
	popup_yn(s);
}

static void make_player(void)
{
	while (1) {
		terminal_clear();

		terminal_color(C_WHITE, C_BLACK);
		terminal_border(0, 0, -1, -1);
		terminal_printf(-1, 0, "[ &Ycreate character&W ]");

		terminal_printf(2, 2, "&YTODO");

		terminal_refresh();
		
		int k = terminal_getc();
		if (k == K_ESCAPE) {
			return;
		}
	}
}

void setup(int load)
{
	// load data here

	load ? load_player() : make_player();
}

