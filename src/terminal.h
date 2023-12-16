#pragma once

#include "palette.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

void terminal_init(void);

void terminal_exit(void);

void terminal_clear(void);

void terminal_refresh(void);

void terminal_color(color_t fore, color_t back);

void terminal_putc(int x, int y, char c);

void terminal_puts(int x, int y, char *s);

int terminal_getc(void);

