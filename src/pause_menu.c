#include "pause_menu.h"

#include "options_menu.h"
#include "help_menu.h"
#include "terminal.h"

int pause_menu(void)
{
	int sel = 0;
	while (1) {
		tclear();

		tcolor(C_WHITE, C_BLACK);
		tborder(0, 0, -1, -1, "paused", NULL);

		tcolor(C_YELLOW, C_BLACK);
		tputc(2, 2 + sel, '\x10');

		tcolor(C_LGRAY, C_BLACK);
		tputs(2 + (sel == 0), 2, "o - options");
		tputs(2 + (sel == 1), 3, "? - help");
		tputs(2 + (sel == 2), 4, "q - quit");

		tflush();
		
		int k = tgetc();
		if (k == K_DOWN) {
			sel = (sel + 1) % 3;
		} else if (k == K_UP) {
			sel = sel > 0 ? (sel - 1) : 2;
		} else if (k == K_ENTER) {
			switch (sel) {
				case 0: {
					options_menu();
				} break;
				case 1: {
					help_menu();
				} break;
				case 2: {
					return 'q';
				}
			}
		} else if (k == 'o') {
			options_menu();
		} else if (k == '?') {
			help_menu();
		} else if (k == 'q') {
			return 'q';
		} else if (k == K_ESCAPE) {
			return '\0';	
		}
	}
}

