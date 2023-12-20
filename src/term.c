#include "term.h"

#include "gl_term.h"
// #include "al_term.h"
#include "nc_term.h"

#include <stdlib.h>
#include <string.h>

int tx = 0;
int ty = 0;

void(*_topen)(void);
void(*_tclose)(void);
void(*_tclear)(void);
void(*_tflush)(void);
void(*_tcolor)(color_t, color_t);
void(*_tputc)(int, int, char);
void(*_tputs)(int, int, char*);
void(*_tprintf)(int, int, char*, va_list);
void(*_tborder)(int, int, int, int, char*, char*);
int(*_tgetc)(void);

void topen(void)
{
	if (strcmp(options_get("display"), "opengl") == 0) {
		_topen		= gl_topen;
		_tclose		= gl_tclose;
		_tclear		= gl_tclear;
		_tflush		= gl_tflush;
		_tcolor		= gl_tcolor;
		_tputc		= gl_tputc;
		_tputs		= gl_tputs;
		_tprintf	= gl_tprintf;
		_tborder	= gl_tborder;
		_tgetc		= gl_tgetc;
	}  else/* if (strcmp(options_get("display"), "allegro") == 0) {
		_topen		= al_topen;
		_tclose		= al_tclose;
		_tclear		= al_tclear;
		_tflush		= al_tflush;
		_tcolor		= al_tcolor;
		_tputc		= al_tputc;
		_tputs		= al_tputs;
		_tprintf	= al_tprintf;
		_tborder	= al_tborder;
		_tgetc		= al_tgetc;
	} else*/ if (strcmp(options_get("display"), "ncurses") == 0) {
		_topen		= nc_topen;
		_tclose		= nc_tclose;
		_tclear		= nc_tclear;
		_tflush		= nc_tflush;
		_tcolor		= nc_tcolor;
		_tputc		= nc_tputc;
		_tputs		= nc_tputs;
		_tprintf	= nc_tprintf;
		_tborder	= nc_tborder;
		_tgetc		= nc_tgetc;
	} else {
		error("no such display mode: %s", options_get("display"));
	}
	
	_topen();
}

void tclose(void)
{
	_tclose();
}

void tclear(void)
{
	_tclear();
}

void tflush(void)
{
	_tflush();
}

void tcolor(color_t fore, color_t back)
{
	_tcolor(fore, back);
}

void tputc(int x, int y, char c)
{
	_tputc(x, y, c);
}

void tputs(int x, int y, char *s)
{
	_tputs(x, y, s);
}

void tprintf(int x, int y, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	_tprintf(x, y, fmt, args);
	va_end(args);
}

void tborder(int x, int y, int w, int h, char *t, char *b)
{
	_tborder(x, y, w, h, t, b);
}

int tgetc(void)
{
	return _tgetc();
}

