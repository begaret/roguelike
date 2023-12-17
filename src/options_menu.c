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
		terminal_clear();

		terminal_color(C_WHITE, C_BLACK);
		terminal_border(0, 0, -1, -1);
		terminal_puts(-1, 0, "\xAE &Yoptions&W \xAF");

		terminal_printf(2 + (sel == 0), 2, "&Wtileset        -   &Y%s", tileset);
		terminal_printf(2 + (sel == 1), 3, "&Wpalette        -   &Y%s", palette);
		terminal_printf(2 + (sel == 2), 5, "&Wwindow_x       -   &Y%s", window_x);
		terminal_printf(2 + (sel == 3), 6, "&Wwindow_y       -   &Y%s", window_y);
		terminal_printf(2 + (sel == 4), 7, "&Wfullscreen     -   &Y%s", fullscreen);

		terminal_color(C_YELLOW, C_BLACK);
		terminal_putc(2, 2 + sel + (sel + 1) / 3, '\x10');

		terminal_refresh();
		
		int k = terminal_getc();
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

