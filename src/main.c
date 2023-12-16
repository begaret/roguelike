#include "options.h"
#include "terminal.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	options_init();
	terminal_init();

	while (1) {
		int k = terminal_getc();
		if (k == -2)
			break;

		terminal_clear();
		terminal_color(C_WHITE, C_LRED);
		terminal_puts(0, 0, "hello, world");
		terminal_color(C_WHITE, C_BLACK);
		terminal_refresh();
	}

	terminal_exit();
	options_exit();

	return 0;
}

