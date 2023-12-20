#include "help_menu.h"

#include "term.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// TODO: add scrolling and make prettier
void help_menu(void)
{
	FILE *fp = fopen("help.txt", "r");
	if (fp == NULL) {
		return;
	}

	tclear();
	
	tcolor(C_WHITE, C_BLACK);
	tborder(0, 0, -1, -1, "help", NULL);

	int y = 2;
	char line[128];
	while (fgets(line, 128, fp) != NULL) {
		tprintf(2, y++, "%.*s", strlen(line) - 1, line);
	}

	tflush();

	fclose(fp);
	while (1) {
		int k = tgetc();
		if (k == K_ESCAPE) {
			return;
		}
	}
}

