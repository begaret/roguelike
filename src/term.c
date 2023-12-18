#include "term.h"

#include "gterm.h"
#include "tterm.h"

#include <stdlib.h>

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
	if (atoi(options_get("display"))) {
		_topen		= _gtopen;
		_tclose		= _gtclose;
		_tclear		= _gtclear;
		_tflush		= _gtflush;
		_tcolor		= _gtcolor;
		_tputc		= _gtputc;
		_tputs		= _gtputs;
		_tprintf	= _gtprintf;
		_tborder	= _gtborder;
		_tgetc		= _gtgetc;
	} else {
		_topen		= _ttopen;
		_tclose		= _ttclose;
		_tclear		= _ttclear;
		_tflush		= _ttflush;
		_tcolor		= _ttcolor;
		_tputc		= _ttputc;
		_tputs		= _ttputs;
		_tprintf	= _ttprintf;
		_tborder	= _ttborder;
		_tgetc		= _ttgetc;
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

