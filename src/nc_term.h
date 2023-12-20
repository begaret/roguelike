#pragma once

#include "options.h"
#include "term.h"

#include <stdarg.h>

void nc_topen(void);

void nc_tclose(void);

void nc_tclear(void);

void nc_tflush(void);

void nc_tcolor(color_t fore, color_t back);

void nc_tputc(int x, int y, char c);

void nc_tputs(int x, int y, char *s);

void nc_tprintf(int x, int y, char *fmt, va_list args);

void nc_tborder(int x, int y, int w, int h, 
			 char *t, char *b);

int nc_tgetc(void);

