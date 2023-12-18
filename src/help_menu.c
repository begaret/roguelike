#include "help_menu.h"

#include "term.h"

#include <stddef.h>

void help_menu(void)
{
	while (1) {
		tclear();

		tcolor(C_WHITE, C_BLACK);
		tborder(0, 0, -1, -1, "help", NULL);

		tprintf(2, 2, "&YTODO");

		tflush();
		
		int k = tgetc();
		if (k == K_ESCAPE) {
			return;
		}
	}
}

