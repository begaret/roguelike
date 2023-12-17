#include "pause_menu.h"

#include "options_menu.h"
#include "help_menu.h"
#include "terminal.h"

#include <stddef.h>

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
		tputs(2 + (sel == 0), 2, "options");
		tputs(2 + (sel == 1), 3, "help");
		tputs(2 + (sel == 2), 4, "quit");

		tflush();
		
		int k = tgetc();
		switch (k) {
			case '8':	case 'k':	sel = sel ? (sel - 1) : 2;	break;
			case '2':	case 'j':	sel = (sel + 1) % 3;		break;
			case K_ENTER: {
				switch (sel) {
					case 0: options_menu(); break;
					case 1: help_menu();	break;
					case 2: return 'q';
				}
			} break;
			case K_ESCAPE:
				return 0;
		}
	}
}

