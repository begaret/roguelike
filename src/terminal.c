#include "terminal.h"

#include "options.h"

static struct {
	ALLEGRO_DISPLAY		*	display;
	ALLEGRO_BITMAP		*	tileset;
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

#ifdef __APPLE__
	terminal.display_x = atoi(options_get("window_x")) * (terminal.tileset_x / 32) * 2;
	terminal.display_y = atoi(options_get("window_y")) * (terminal.tileset_y / 8) * 2;
#else
	terminal.display_x = atoi(options_get("window_x")) * (terminal.tileset_x / 32);
	terminal.display_y = atoi(options_get("window_y")) * (terminal.tileset_y / 8);
#endif

	terminal.display = al_create_display(terminal.display_x, terminal.display_y);
	if (terminal.display == NULL) {
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

	al_hold_bitmap_drawing(1);
}

void terminal_exit(void)
{
	al_destroy_event_queue(terminal.queue);
	al_destroy_timer(terminal.timer);
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
	al_flip_display();
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

	unsigned u = c % 32;
	unsigned v = c / 32;

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

int terminal_getc(void)
{
	ALLEGRO_EVENT event;
	al_wait_for_event(terminal.queue, &event);
	
	switch (event.type) {
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			return -2;
		case ALLEGRO_EVENT_KEY_CHAR:
			return event.keyboard.unichar;
	}

	return -1;
}

