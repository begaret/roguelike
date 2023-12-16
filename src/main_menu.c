#include "main_menu.h"

#include "terminal.h"

int main_menu(void)
{
	int sel = 0;
	while (1) {
		terminal_clear();

		terminal_color(C_LMAGENTA, C_BLACK);
		terminal_puts(1, 1, "F U T U R A");
		
		terminal_color(C_YELLOW, C_BLACK);
		terminal_putc(1, 3 + sel, '\x10');

		terminal_color(C_WHITE, C_BLACK);
		terminal_puts(1 + (sel == 0), 3, "new game");
		terminal_puts(1 + (sel == 1), 4, "load game");
		terminal_puts(1 + (sel == 2), 5, "create world");
		terminal_puts(1 + (sel == 3), 6, "options");
		terminal_puts(1 + (sel == 4), 7, "quit");

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
				case 2: return 'w';
				case 3: return 'o';
				case 4: return 'q';
			}
		} else if (k == 'n' 
				|| k == 'l' 
				|| k == 'w' 
				|| k == 'o' 
				|| k == 'q') {
			return k;
		}
	};
}

