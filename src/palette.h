#pragma once

#define C_BLACK		0x00
#define C_BLUE		0x01
#define C_GREEN		0x02
#define C_CYAN		0x03
#define C_RED		0x04
#define C_MAGENTA	0x05
#define C_BROWN		0x06
#define C_LGRAY		0x07
#define C_GRAY		0x08
#define C_LBLUE		0x09
#define C_LGREEN	0x0A
#define C_LCYAN		0x0B
#define C_LRED		0x0C
#define C_LMAGENTA	0x0D
#define C_YELLOW	0x0E
#define C_WHITE		0x0F

typedef unsigned char color_t;

void palette_init(void);

unsigned palette_get(color_t color);

