#include "options_menu.h"

#include "options.h"
#include "terminal.h"
#include "popup.h"

void options_menu(void)
{
	char *tileset		= options_get("tileset");
	char *palette		= options_get("palette");
	char *window_x		= options_get("window_x");
	char *window_y		= options_get("window_y");
	char *fullscreen	= options_get("fullscreen");

	int changed = 0;
	int sel = 0;

	while (1) {
		tclear();

		tcolor(C_WHITE, C_BLACK);
		tborder(0, 0, -1, -1);
		tputs(-1, 0, "\xAE &Yoptions&W \xAF");

		tprintf(2 + (sel == 0), 2, "&wtileset");
		tprintf(2 + (sel == 1), 3, "&wpalette");
		tprintf(2 + (sel == 2), 5, "&wwindow_x");
		tprintf(2 + (sel == 3), 6, "&wwindow_y");
		tprintf(2 + (sel == 4), 7, "&wfullscreen");

		tprintf(18, 2, "&W-   &Y%s", tileset);
		tprintf(18, 3, "&W-   &Y%s", palette);
		tprintf(18, 5, "&W-   &Y%s", window_x);
		tprintf(18, 6, "&W-   &Y%s", window_y);
		tprintf(18, 7, "&W-   &Y%s", fullscreen);

		tcolor(C_YELLOW, C_BLACK);
		tputc(2, 2 + sel + (sel + 1) / 3, '\x10');

		tflush();
		
		int k = tgetc();
		if (k == K_DOWN) {
			sel = (sel + 1) % 5;
		} else if (k == K_UP) {
			sel = sel > 0 ? (sel - 1) : 4;
		} else if (k == K_ENTER) {

		} else if (k == K_ESCAPE) {
			if (!changed) {
				return;
			}

			if (popup_yn("save changes?")) {
				FILE *fp = fopen("options.txt", "w");
				fprintf(fp, "tileset[%s]\n",	tileset);
				fprintf(fp, "palette[%s]\n",	palette);
				fprintf(fp, "window_x[%s]\n",	window_x);
				fprintf(fp, "window_y[%s]\n",	window_y);
				fprintf(fp, "fullscreen[%s]\n", fullscreen);
				fclose(fp);
				return;
			}
		}
	}
}

