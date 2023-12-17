#include "terminal.h"

#include "options.h"

#include <stdlib.h>
#include <stdarg.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

static struct {
	ALLEGRO_DISPLAY		*	display;
	ALLEGRO_BITMAP		*	tileset;
	ALLEGRO_BITMAP		*	buffer;
	ALLEGRO_TIMER		*	timer;
	ALLEGRO_EVENT_QUEUE *	queue;

	unsigned				display_x;
	unsigned				display_y;
	unsigned				tileset_x;
	unsigned				tileset_y;
	unsigned				window_x;
	unsigned				window_y;

	color_t					fore;
	color_t					back;
} terminal;

int tx = 0;
int ty = 0;

void topen(void)
{
	if (al_init() == 0 || al_init_image_addon() == 0) {
		return;
	}

	al_install_keyboard();
	al_install_mouse();

	terminal.tileset = al_load_bitmap(options_get("tileset"));
	if (terminal.tileset == NULL) {
		return;
	}

	terminal.tileset_x = al_get_bitmap_width(terminal.tileset);
	terminal.tileset_y = al_get_bitmap_height(terminal.tileset);

	terminal.window_x = atoi(options_get("window_x"));
	terminal.window_y = atoi(options_get("window_y"));

	if (!atoi(options_get("windowed"))) {
		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	} else {
#ifdef __APPLE__
	terminal.display_x = terminal.window_x * (terminal.tileset_x / 32) * 2;
	terminal.display_y = terminal.window_y * (terminal.tileset_y / 8) * 2;
#else
	terminal.display_x = terminal.window_x * (terminal.tileset_x / 32);
	terminal.display_y = terminal.window_y * (terminal.tileset_y / 8);
#endif
	}

	terminal.display = al_create_display(terminal.display_x, terminal.display_y);
	if (terminal.display == NULL) {
		return;
	}

	if (!atoi(options_get("windowed"))) {
		terminal.display_x = al_get_display_width(terminal.display);
		terminal.display_y = al_get_display_height(terminal.display);
		
#ifdef __APPLE__
		terminal.window_x = terminal.display_x / (terminal.tileset_x / 32) / 2;
		terminal.window_y = terminal.display_y / (terminal.tileset_y / 8)  / 2;
#else
		terminal.window_x = terminal.display_x / (terminal.tileset_x / 32);
		terminal.window_y = terminal.display_y / (terminal.tileset_y / 8);
#endif
	}

#ifdef __APPLE__
	terminal.buffer = al_create_bitmap(terminal.display_x / 2, terminal.display_y / 2);
#else
	terminal.buffer = al_create_bitmap(terminal.display_x, terminal.display_y);
#endif
	if (terminal.buffer == NULL) {
		return;
	}

	al_convert_mask_to_alpha(terminal.tileset, al_map_rgb(0x00, 0x00, 0x00));

	terminal.timer = al_create_timer(1.0 / 60.0);
	if (terminal.timer == NULL) {
		return;
	}

	terminal.queue = al_create_event_queue();
	if (terminal.queue == NULL) {
		return;
	}

	al_register_event_source(terminal.queue, al_get_keyboard_event_source());
	al_register_event_source(terminal.queue, al_get_mouse_event_source());
	al_register_event_source(terminal.queue, al_get_display_event_source(terminal.display));
	al_register_event_source(terminal.queue, al_get_timer_event_source(terminal.timer));

	al_start_timer(terminal.timer);

	palette_init();
	terminal.fore = C_WHITE;
	terminal.back = C_BLACK;

	al_set_target_bitmap(terminal.buffer);
	al_hold_bitmap_drawing(1);

	tx = terminal.window_x;
	ty = terminal.window_y;

	static char title[128];
	snprintf(title, 128, "%ix%i", tx, ty);

	al_set_window_title(terminal.display, title);
}

void tclose(void)
{
	al_destroy_event_queue(terminal.queue);
	al_destroy_timer(terminal.timer);
	al_destroy_bitmap(terminal.buffer);
	al_destroy_display(terminal.display);
	al_destroy_bitmap(terminal.tileset);
}

void tclear(void)
{
	unsigned color = palette_get(terminal.back);
	al_clear_to_color(al_map_rgb(
		(color >> 16) & 0xFF, 
		(color >>  8) & 0xFF,
		(color >>  0) & 0xFF)
	);
}

void tflush(void)
{
	al_hold_bitmap_drawing(0);
	al_set_target_backbuffer(terminal.display);
	al_draw_scaled_bitmap(terminal.buffer,
#ifdef __APPLE__
		0, 0, terminal.display_x / 2, terminal.display_y / 2,
#else
		0, 0, terminal.display_x, terminal.display_y,
#endif
		0, 0, terminal.display_x, terminal.display_y,
	0);

	al_flip_display();
	al_set_target_bitmap(terminal.buffer);
	al_hold_bitmap_drawing(1);
}

void tcolor(color_t fore, color_t back)
{
	terminal.fore = fore;
	terminal.back = back;
}

void tputc(int x, int y, char c)
{
	if (x < 0 || x >= (int)terminal.window_x
	 || y < 0 || y >= (int)terminal.window_y) {
		return;
	}

	unsigned color1 = palette_get(terminal.fore);
	unsigned color2 = palette_get(terminal.back);

	unsigned u = (c & 0xFF) % 32;
	unsigned v = (c & 0xFF) / 32;

	unsigned w = terminal.tileset_x / 32;
	unsigned h = terminal.tileset_y / 8;

	al_draw_tinted_bitmap_region(
		terminal.tileset,
		al_map_rgb(
			(color2 >> 16) & 0xFF,
			(color2 >>  8) & 0xFF,
			(color2 >>  0) & 0xFF),
		27 * w, 6 * h, 
		w, 		h, 
		x * w,	y * h, 0
	);

	al_draw_tinted_bitmap_region(
		terminal.tileset,
		al_map_rgb(
			(color1 >> 16) & 0xFF,
			(color1 >>  8) & 0xFF,
			(color1 >>  0) & 0xFF),
		u * w, 	v * h, 
		w, 		h, 
		x * w,	y * h, 0
	);
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

void tputs(int x, int y, char *s)
{
	int ac = 0;
	char *p = s;
	for (ac = 0; p[ac]; p[ac] == '&' ? ac++ : *p++);

	if (x == -1) {
		x = terminal.window_x / 2 - strlen(s) / 2 + ac;
	}

	while (*s) {
		if (*s == '&') {
			if (*++s == '&') {
				tputc(x++, y, *s++);
			} else {
				tcolor(ch_to_color(*s++), terminal.back);
			}
		} else {
			tputc(x++, y, *s++);
		}
	}
}

void tprintf(int x, int y, char *fmt, ...)
{
	static char s[256];

	va_list args;
	va_start(args, fmt);
	vsnprintf(s, 255, fmt, args);
	va_end(args);

	tputs(x, y, s);
}

void tborder(int x, int y, int w, int h, char *t, char *b)
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

	tputc(x    , y    , '\xDA');
	tputc(x + w, y    , '\xBF');
	tputc(x + w, y + h, '\xD9');
	tputc(x    , y + h, '\xC0');
	for (int i = x + 1; i < x + w; i++) {
		tputc(i, y    , '\xC4');
		tputc(i, y + h, '\xC4');
	}

	for (int i = y + 1; i < y + h; i++) {
		tputc(x    , i, '\xB3');
		tputc(x + w, i, '\xB3');
	}

	for (int X = x + 1; X < x + w; X++) {
		for (int Y = y + 1; Y < y + h; Y++) {
			tputc(X, Y, ' ');
		}
	}

	if (t) {
		const int l = strlen(t) + 4;
		tprintf(x + w / 2 - l / 2, y, "\xAE &Y%s&W \xAF", t);
	}

	if (b) {
		const int l = strlen(b) + 4;
		tprintf(x + w / 2 - l / 2, y + h, "\xAE &Y%s&W \xAF", b);
	}
}

static int convert_keycode(int code)
{
	switch (code) {
		case ALLEGRO_KEY_LEFT:		return K_LEFT;
		case ALLEGRO_KEY_RIGHT:		return K_RIGHT;
		case ALLEGRO_KEY_UP:		return K_UP;
		case ALLEGRO_KEY_DOWN:		return K_DOWN;
		default:
			return 0;
	}
}

static int convert_unicode(int code)
{
	switch (code) {	
		case 0x09: return K_TAB;
		case 0x0D: return K_ENTER;
		case 0x1B: return K_ESCAPE;
		case 0x7F: return K_DELETE;

		case 0xA7: return 0x15;	// §

		case 0xC4: return 0x8E;	// Å
		case 0xC5: return 0x8F;	// Ä
		case 0xD6: return 0x99;	// Ö

		case 0xE4: return 0x84;	// å
		case 0xE5: return 0x86;	// ä
		case 0xF6: return 0x94;	// ö
		default:
			return code;
	}
}

int tgetc(void)
{
	ALLEGRO_EVENT event;
	al_wait_for_event(terminal.queue, &event);

	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		tclose();
		exit(EXIT_SUCCESS);
	} else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
		int k = event.keyboard.unichar;
		if (k <= 0) {
			return convert_keycode(event.keyboard.keycode);
		}

		return convert_unicode(k);
	} else if (event.type == ALLEGRO_EVENT_TIMER) {
		return K_ERR;
	}

	return K_ERR;
}

