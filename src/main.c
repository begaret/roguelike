#include "terminal.h"
#include "options.h"
#include "options_menu.h"
#include "main_menu.h"
#include "help_menu.h"

void quit(void)
{
	terminal_exit();
	options_exit();

	atexit(quit);
}

void init(void)
{
	options_init();
	terminal_init();
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
			case 'n': break;
			case 'l': break;
			case 'w': break;
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

