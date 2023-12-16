#pragma once

#include "palette.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

enum key_t
{
	K_ERR	= -1,

	K_LEFT	= 0x100,
	K_RIGHT	= 0x101,
	K_UP	= 0x102,
	K_DOWN	= 0x103,

	K_ENTER		= 0x104,
	K_TAB		= 0x105,
	K_DELETE	= 0x106,
	K_ESCAPE	= 0x107,
};

void terminal_init(void);

void terminal_exit(void);

void terminal_clear(void);

void terminal_refresh(void);

void terminal_color(color_t fore, color_t back);

void terminal_putc(int x, int y, char c);

void terminal_puts(int x, int y, char *s);

void terminal_printf(int x, int y, char *fmt, ...);

void terminal_border(int x, int y, int w, int h);

int terminal_getc(void);

void terminal_size(unsigned *w, unsigned *h);

