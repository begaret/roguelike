#include "main_menu.h"

#include "popup.h"
#include "terminal.h"

int main_menu(void)
{
	int sel = 0;
	while (1) {
		terminal_clear();

		terminal_puts(1, 1, "&Wroguelike &wv0.0.0");
		
		terminal_color(C_YELLOW, C_BLACK);
		terminal_putc(1, 3 + sel, '\x10');

		terminal_color(C_WHITE, C_BLACK);
		terminal_puts(1 + (sel == 0), 3, "n - new game");
		terminal_puts(1 + (sel == 1), 4, "l - load game");
		terminal_puts(1 + (sel == 2), 5, "o - options");
		terminal_puts(1 + (sel == 3), 6, "? - help");
		terminal_puts(1 + (sel == 4), 7, "q - quit");

		terminal_refresh();
		
		int k = terminal_getc();
		if (k == K_DOWN) {
			sel = (sel + 1) % 5;
		} else if (k == K_UP) {
			sel = sel > 0 ? (sel - 1) : 4;
		} else if (k == K_ENTER) {
			switch (sel) {
				case 0: return 'n';
				case 1: return 'l';
				case 2: return 'o';
				case 3: return '?';
				case 4: return 'q';
			}
		} else if (k == 'n' 
				|| k == 'l' 
				|| k == 'o'
				|| k == '?'
				|| k == 'q') {
			return k;
		} else if (k == K_ESCAPE) {
			if (popup_yn("really quit?"))
				return 'q';
		}
	}
}

