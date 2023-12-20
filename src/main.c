#include "term.h"
#include "options.h"
#include "options_menu.h"
#include "main_menu.h"
#include "help_menu.h"
#include "game.h"

#include <stdlib.h>

// #ifdef __APPLE__
//	#include <allegro5/allegro.h>
// #endif

void quit(void)
{
	tclose();
	options_exit();
}

void init(void)
{
	options_init();
	topen();

	atexit(quit);
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	init();
	while (1) {
		int o = main_menu();
		if (o == 'q') {
			break;
		}

		switch (o) {
			case 'n': setup(0); break;
			case 'l': setup(1); break;
			case 'o': {
				options_menu(); 
			} break;
			case '?': {
				help_menu();
			} break;
		}
	}

	exit(EXIT_SUCCESS);
}

