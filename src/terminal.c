#include "terminal.h"

#include "options.h"

#include <stdlib.h>
#include <stdarg.h>

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

	color_t					fore;
	color_t					back;
} terminal;

void terminal_init(void)
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

	if (atoi(options_get("fullscreen"))) {
		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	} else {
#ifdef __APPLE__
	terminal.display_x = atoi(options_get("window_x")) * (terminal.tileset_x / 32) * 2;
	terminal.display_y = atoi(options_get("window_y")) * (terminal.tileset_y / 8) * 2;
#else
	terminal.display_x = atoi(options_get("window_x")) * (terminal.tileset_x / 32);
	terminal.display_y = atoi(options_get("window_y")) * (terminal.tileset_y / 8);
#endif
	}

	terminal.display = al_create_display(terminal.display_x, terminal.display_y);
	if (terminal.display == NULL) {
		return;
	}

	if (atoi(options_get("fullscreen"))) {
		terminal.display_x = al_get_display_width(terminal.display);
		terminal.display_y = al_get_display_height(terminal.display);
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
}

void terminal_exit(void)
{
	al_destroy_event_queue(terminal.queue);
	al_destroy_timer(terminal.timer);
	al_destroy_bitmap(terminal.buffer);
	al_destroy_display(terminal.display);
	al_destroy_bitmap(terminal.tileset);
}

void terminal_clear(void)
{
	unsigned color = palette_get(terminal.back);
	al_clear_to_color(al_map_rgb(
		(color >> 16) & 0xFF, 
		(color >>  8) & 0xFF,
		(color >>  0) & 0xFF)
	);
}

void terminal_refresh(void)
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

void terminal_color(color_t fore, color_t back)
{
	terminal.fore = fore;
	terminal.back = back;
}

void terminal_putc(int x, int y, char c)
{
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

void terminal_puts(int x, int y, char *s)
{
	while (*s) {
		terminal_putc(x++, y, *s++);
	}
}

void terminal_printf(int x, int y, char *fmt, ...)
{
	static char s[256];

	va_list args;
	va_start(args, fmt);
	vsnprintf(s, 255, fmt, args);
	va_end(args);

	terminal_puts(x, y, s);
}

static int convert_keycode(int code)
{
	switch (code) {
		case ALLEGRO_KEY_LEFT:	return K_LEFT;
		case ALLEGRO_KEY_RIGHT:	return K_RIGHT;
		case ALLEGRO_KEY_UP:	return K_UP;
		case ALLEGRO_KEY_DOWN:	return K_DOWN;
		case ALLEGRO_KEY_ENTER: return K_ENTER;
		default:
			return 0;
	}
}

static int convert_unicode(int code)
{
	switch (code) {	
		case 0x09: return K_TAB;
		case 0x0D: return K_ENTER;
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

int terminal_getc(void)
{
	ALLEGRO_EVENT event;
	al_wait_for_event(terminal.queue, &event);

	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		terminal_exit();
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

void terminal_size(unsigned *w, unsigned *h)
{
	if (w) {
		*w = terminal.display_x / (terminal.tileset_x / 32) / 2;
	}

	if (h) {
		*h = terminal.display_y / (terminal.tileset_y / 8) / 2;
	}
}

