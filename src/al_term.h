#pragma once

#include "options.h"
#include "term.h"

#include <stdarg.h>

void al_topen(void);

void al_tclose(void);

void al_tclear(void);

void al_tflush(void);

void al_tcolor(color_t fore, color_t back);

void al_tputc(int x, int y, char c);

void al_tputs(int x, int y, char *s);

void al_tprintf(int x, int y, char *fmt, va_list args);

void al_tborder(int x, int y, int w, int h, 
			 char *t, char *b);

int al_tgetc(void);

