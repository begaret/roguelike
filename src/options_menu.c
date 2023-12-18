#include "options_menu.h"

#include "options.h"
#include "terminal.h"
#include "popup.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void options_menu(void)
{
	struct {char name[64]; char *s;} opt[] = {
		{"tileset",		NULL},
		{"palette",		NULL},
		{"window_x",	NULL},
		{"window_y",	NULL},
		{"windowed",	NULL},
	};

	int changed = 0;
	int sel = 0;

	int len = sizeof(opt) / sizeof(opt[0]);
	while (1) {
		tclear();

		tcolor(C_WHITE, C_BLACK);
		tborder(0, 0, -1, -1, "options", NULL);

		tcolor(C_YELLOW, C_BLACK);
		tputc(2, 2 + sel + (sel + 1) / 3, '\x10');

		for (int i = 0; i < len; i++) {
			tprintf(2 + (sel == i), 2 + i + (i + 1) / 3, "&w%s", opt[i].name);
			tprintf(18, 2 + i + (i + 1) / 3, "&W-   &Y%s", 
				opt[i].s ? opt[i].s : options_get(opt[i].name));
		}

		tflush();
		
		int k = tgetc();
		switch (k) {
			case '8':	case 'k':	sel = sel ? (sel - 1) : len - 1;	break;
			case '2':	case 'j':	sel = (sel + 1) % len;				break;
			case K_ENTER: {
				char *s = popup_s("enter value");
				if (s) {
					free(opt[sel].s);
					opt[sel].s = strdup(s);
					changed = 1;
				}
			} break;
			case K_ESCAPE: {
				if (!changed) {
					return;
				}

				if (popup_yn("save changes?")) {
					FILE *fp = fopen("options.txt", "w");
					for (int i = 0; i < len; i++) {
						fprintf(fp, "%s[%s]\n",	opt[i].name, 
							opt[i].s ? opt[i].s : options_get(opt[i].name));
					}

					fclose(fp);
				}

				for (int i = 0; i < len; i++) {
					free(opt[i].s);
				}
				
				return;
			} break;
		}
	}
}

