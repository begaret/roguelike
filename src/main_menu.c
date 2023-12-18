#include "main_menu.h"

#include "popup.h"
#include "term.h"

int main_menu(void)
{
	int sel = 0;
	while (1) {
		tclear();

		tputs(1, 1, "&Wroguelike &wv0.0.0");
		tcolor(C_YELLOW, C_BLACK);
		tputc(1, 3 + sel, '\x10');

		tcolor(C_LGRAY, C_BLACK);
		tputs(1 + (sel == 0), 3, "new game");
		tputs(1 + (sel == 1), 4, "load game");
		tputs(1 + (sel == 2), 5, "options");
		tputs(1 + (sel == 3), 6, "help");
		tputs(1 + (sel == 4), 7, "quit");

		tflush();
		
		int k = tgetc();
		switch (k) {
			case '8':	case 'k':	sel = sel ? (sel - 1) : 4;	break;
			case '2':	case 'j':	sel = (sel + 1) % 5;		break;
			case K_ENTER: {
				switch (sel) {
					case 0: return 'n';
					case 1: return 'l';
					case 2: return 'o';
					case 3: return '?';
					case 4: return 'q';
				}
			} break;
			case K_ESCAPE: {
				if (popup_yn("really quit?"))
					return 'q';
			} break;
		}
	}
}

