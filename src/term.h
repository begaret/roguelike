#pragma once

#include "palette.h"

enum key_t
{
	K_ERR		= -1,

	K_LEFT		= 0x100,
	K_RIGHT		= 0x101,
	K_UP		= 0x102,
	K_DOWN		= 0x103,

	K_ENTER		= 0x104,
	K_TAB		= 0x105,
	K_DELETE	= 0x106,
	K_ESCAPE	= 0x107,
};

extern int tx;
extern int ty;

void topen(void);

void tclose(void);

void tclear(void);

void tflush(void);

void tcolor(color_t fore, color_t back);

void tputc(int x, int y, char c);

void tputs(int x, int y, char *s);

void tprintf(int x, int y, char *fmt, ...);

void tborder(int x, int y, int w, int h, 
			 char *t, char *b);

int tgetc(void);

