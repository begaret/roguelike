#include "terminal.h"
#include "options.h"
#include "main_menu.h"

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
	}

	exit(EXIT_SUCCESS);
}

