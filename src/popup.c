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
		tborder(-1, -1, w, h, NULL, "y/n");
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
		tborder(-1, -1, w + i, h, NULL, NULL);

		tprintf(-1, y + 2, "%s: %s", m, s);

		tflush();

		int k = tgetc();
		switch (k) {
			case 0 ... 0x100: {
				if (i < 127) s[i++] = k;
			} break;
			case K_DELETE: {
				if (i > 0) s[--i] = '\0';
			} break;
			case K_ENTER: {
				return s;
			} break;
			case K_ESCAPE: {
				return NULL;
			} break;
		}
	}
}

