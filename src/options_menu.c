#include "options_menu.h"

#include "options.h"
#include "terminal.h"

void options_menu(void)
{
	while (1) {
		terminal_clear();

		terminal_color(C_WHITE, C_BLACK);
		terminal_border(0, 0, -1, -1);
		terminal_printf(-1, 0, "[ &Yoptions&W ]");

		terminal_printf(2, 2, "&YTODO");

		terminal_refresh();
		
		int k = terminal_getc();
		if (k == K_ESCAPE) {
			return;
		}
	}
}

