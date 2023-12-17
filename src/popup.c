#include "popup.h"

#include "terminal.h"

#include <string.h>

int popup_yn(char *m)
{
	unsigned win_y;
	terminal_size(NULL, &win_y);

	int w = strlen(m) + 4;
	w > 16 ?: (w = 16);
	int h = 1 + 3;

	int y = win_y / 2 - h / 2;
	while (1) {
		terminal_color(C_WHITE, C_BLACK);
		terminal_border(-1, -1, w, h);
		
		terminal_puts(-1, y + 4, "\xAE &Yy/n&W \xAF");
		terminal_puts(-1, y + 2, m);

		terminal_refresh();

		int k = terminal_getc();
		if (k == 'y' || k == K_ENTER) {
			return 1;
		}

		if (k == 'n' || k == K_ESCAPE) {
			return 0;
		}
	}
}

char *popup_s(char *m)
{
	unsigned win_y;
	terminal_size(NULL, &win_y);

	int w = strlen(m) + 2 + 4;
	w > 16 ?: (w = 16);
	int h = 1 + 3;

	int y = win_y / 2 - h / 2;
	
	static char s[128];
	memset(s, 0, 128);

	int i = 0;
	while (1) {
		terminal_color(C_WHITE, C_BLACK);
		terminal_border(-1, -1, w + i, h);

		terminal_printf(-1, y + 2, "%s: %s", m, s);

		terminal_refresh();

		int k = terminal_getc();
		if (k >= 0 && k < 0x100 && i < 127) {
			s[i++] = k;
		} else if (k == K_DELETE && i > 0) {
			s[--i] = '\0';
		} else if (k == K_ENTER) {
			return s;
		}
	}
}

