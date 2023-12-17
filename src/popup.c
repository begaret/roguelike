#include "popup.h"

#include "terminal.h"

#include <string.h>

int popup_yn(char *m)
{
	int w = strlen(m) + 4;
	w > 16 ?: (w = 16);
	int h = 1 + 3;

	int y = ty / 2 - h / 2;
	while (1) {
		tcolor(C_WHITE, C_BLACK);
		tborder(-1, -1, w, h);
		
		tputs(-1, y + 4, "\xAE &Yy/n&W \xAF");
		tputs(-1, y + 2, m);

		tflush();

		int k = tgetc();
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
	int w = strlen(m) + 2 + 4;
	w > 16 ?: (w = 16);
	int h = 1 + 3;

	int y = ty / 2 - h / 2;
	
	static char s[128];
	memset(s, 0, 128);

	int i = 0;
	while (1) {
		tcolor(C_WHITE, C_BLACK);
		tborder(-1, -1, w + i, h);

		tprintf(-1, y + 2, "%s: %s", m, s);

		tflush();

		int k = tgetc();
		if (k >= 0 && k < 0x100 && i < 127) {
			s[i++] = k;
		} else if (k == K_DELETE && i > 0) {
			s[--i] = '\0';
		} else if (k == K_ENTER) {
			return s;
		}
	}
}

