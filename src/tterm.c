#include "tterm.h"

#include "options.h"

#include <stdlib.h>
#include <string.h>

#include <ncurses.h>
#include <locale.h>

static int escdelay = 0;

void _ttopen(void)
{
	setlocale(LC_ALL, "");
	initscr();
	raw();
	noecho();
	cbreak();
	keypad(stdscr, 1);
	curs_set(0);

	start_color();
	for (unsigned char c1 = 0; c1 < 0x10; c1++) {
		for (unsigned char c2 = 0; c2 < 0x10; c2++) {
			init_pair((c2 | (c1 << 4)) + 1, c1, c2);
		}
	}

	getmaxyx(stdscr, ty, tx);

	escdelay = ESCDELAY;
	set_escdelay(0);
}

void _ttclose(void)
{
	set_escdelay(escdelay);

	curs_set(1);
	keypad(stdscr, 0);
	nocbreak();
	echo();
	noraw();
	endwin();
}

void _ttclear(void)
{
	erase();
}

void _ttflush(void)
{
	refresh();
}

static color_t convert_color(color_t color)
{
	switch (color) {
		case C_BLACK:		return 0x00;
		case C_BLUE:		return 0x04;
		case C_GREEN:		return 0x02;
		case C_CYAN:		return 0x06;
		case C_RED:			return 0x01;
		case C_MAGENTA:		return 0x05;
		case C_BROWN:		return 0x03;
		case C_LGRAY:		return 0x07;

		case C_GRAY:		return 0x08;
		case C_LBLUE:		return 0x0C;
		case C_LGREEN:		return 0x0A;
		case C_LCYAN:		return 0x0E;
		case C_LRED:		return 0x09;
		case C_LMAGENTA:	return 0x0D;
		case C_YELLOW:		return 0x0B;
		case C_WHITE:		return 0x0F;

		default:
			return 0x00;
	}
}

void _ttcolor(color_t fore, color_t back)
{
	attron(COLOR_PAIR((convert_color(back) | (convert_color(fore) << 4)) + 1));
}

static char *convert_char(char c)
{
	static char wc[4] = {0};
	switch (c) {
		case '\x10':	{ wc[0] = 0xE2; wc[1] = 0x96; wc[2] = 0xBA; } break;
		case '\x11':	{ wc[0] = 0xE2; wc[1] = 0x97; wc[2] = 0x84; } break;
		default: {
			wc[0] = '?';
			wc[1] = 0;
		} break; 
	}

	return wc;
}

void _ttputc(int x, int y, char c)
{
	if (c >= 0x20 && c <= 0x7E) {
		mvaddch(y, x, c);
		return;
	}

	mvaddstr(y, x, convert_char(c));	
}

static color_t ch_to_color(char c)
{
	switch (c) {
		case 'k':	return C_BLACK;
		case 'b':	return C_BLUE;
		case 'g':	return C_GREEN;
		case 'c':	return C_CYAN;
		case 'r':	return C_RED;
		case 'm':	return C_MAGENTA;
		case 'y':	return C_BROWN;
		case 'w':	return C_LGRAY;	
		case 'K':	return C_GRAY;
		case 'B':	return C_LBLUE;
		case 'G':	return C_LGREEN;
		case 'C':	return C_LCYAN;
		case 'R':	return C_LRED;
		case 'M':	return C_LMAGENTA;
		case 'Y':	return C_YELLOW;
		case 'W':	return C_WHITE;
		case '*':	return rand() % 0x10;
		default:
			return C_GRAY;
	}
}

void _ttputs(int x, int y, char *s)
{
	int ac = 0;
	char *p = s;
	for (ac = 0; p[ac]; p[ac] == '&' ? ac++ : *p++);

	if (x == -1) {
		x = tx / 2 - strlen(s) / 2 + ac;
	}

	while (*s) {
		if (*s == '&') {
			if (*++s == '&') {
				_ttputc(x++, y, *s++);
			} else {
				_ttcolor(ch_to_color(*s++), C_BLACK);
			}
		} else {
			_ttputc(x++, y, *s++);
		}
	}
}

void _ttprintf(int x, int y, char *fmt, va_list args)
{
	static char s[256];
	vsnprintf(s, 255, fmt, args);

	_ttputs(x, y, s);
}

// TODO: rewrite this
static void _ttprintfs(int x, int y, char *fmt, ...)
{
	static char s[256];

	va_list args;
	va_start(args, fmt);
	vsnprintf(s, 255, fmt, args);
	va_end(args);

	_ttputs(x, y, s);
}

void _ttborder(int x, int y, int w, int h, char *t, char *b)
{
	if (w == -1) {
		w = tx - 1;
	}

	if (h == -1) {
		h = ty - 1;
	}

	if (x == -1) {
		x = tx / 2 - w / 2;
	}

	if (y == -1) {
		y = ty / 2 - h / 2;
	}

	_ttputc(x    , y    , '\xDA');
	_ttputc(x + w, y    , '\xBF');
	_ttputc(x + w, y + h, '\xD9');
	_ttputc(x    , y + h, '\xC0');
	for (int i = x + 1; i < x + w; i++) {
		_ttputc(i, y    , '\xC4');
		_ttputc(i, y + h, '\xC4');
	}

	for (int i = y + 1; i < y + h; i++) {
		_ttputc(x    , i, '\xB3');
		_ttputc(x + w, i, '\xB3');
	}

	for (int X = x + 1; X < x + w; X++) {
		for (int Y = y + 1; Y < y + h; Y++) {
			_ttputc(X, Y, ' ');
		}
	}

	if (t) {
		const int l = strlen(t) + 4;
		_ttprintfs(x + w / 2 - l / 2 + 1, y,
			"\xAE &Y%s&W \xAF", t);
	}

	if (b) {
		const int l = strlen(b) + 4;
		_ttprintfs(x + w / 2 - l / 2, y + h, 
			"\xAE &Y%s&W \xAF", b);
	}
}

static int convert_key(int k)
{
	switch (k) {
		case 0x09:	return K_TAB;
		case 0x0A:	return K_ENTER;
		case 0x1B:	return K_ESCAPE;
		case 0x7F:	return K_DELETE;

		case KEY_LEFT:	return K_LEFT;
		case KEY_RIGHT:	return K_RIGHT;
		case KEY_UP:	return K_UP;
		case KEY_DOWN:	return K_DOWN;

		case ERR:	return K_ERR;
		default:
			return k;
	}
}

int _ttgetc(void)
{
	return convert_key(getch());
}

