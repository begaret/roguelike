#pragma once

#include "options.h"
#include "term.h"

#include <stdarg.h>

void _gtopen(void);

void _gtclose(void);

void _gtclear(void);

void _gtflush(void);

void _gtcolor(color_t fore, color_t back);

void _gtputc(int x, int y, char c);

void _gtputs(int x, int y, char *s);

void _gtprintf(int x, int y, char *fmt, va_list args);

void _gtborder(int x, int y, int w, int h, 
			 char *t, char *b);

int _gtgetc(void);

