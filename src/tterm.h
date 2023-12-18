#pragma once

#include "options.h"
#include "term.h"

#include <stdarg.h>

void _ttopen(void);

void _ttclose(void);

void _ttclear(void);

void _ttflush(void);

void _ttcolor(color_t fore, color_t back);

void _ttputc(int x, int y, char c);

void _ttputs(int x, int y, char *s);

void _ttprintf(int x, int y, char *fmt, va_list args);

void _ttborder(int x, int y, int w, int h, 
			 char *t, char *b);

int _ttgetc(void);

