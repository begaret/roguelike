#pragma once

#include "options.h"
#include "term.h"

#include <stdarg.h>

void gl_topen(void);

void gl_tclose(void);

void gl_tclear(void);

void gl_tflush(void);

void gl_tcolor(color_t fore, color_t back);

void gl_tputc(int x, int y, char c);

void gl_tputs(int x, int y, char *s);

void gl_tprintf(int x, int y, char *fmt, va_list args);

void gl_tborder(int x, int y, int w, int h, 
			 char *t, char *b);

int gl_tgetc(void);

