#include "main_menu.h"

#include "popup.h"
#include "terminal.h"

int main_menu(void)
{
	int sel = 0;
	while (1) {
		tclear();

		tputs(1, 1, "&Wroguelike &wv0.0.0");
		tcolor(C_YELLOW, C_BLACK);
		tputc(1, 3 + sel, '\x10');

		tcolor(C_LGRAY, C_BLACK);
		tputs(1 + (sel == 0), 3, "n - new game");
		tputs(1 + (sel == 1), 4, "l - load game");
		tputs(1 + (sel == 2), 5, "o - options");
		tputs(1 + (sel == 3), 6, "? - help");
		tputs(1 + (sel == 4), 7, "q - quit");

		tflush();
		
		int k = tgetc();
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

