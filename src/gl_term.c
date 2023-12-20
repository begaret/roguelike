#include "gl_term.h"

#include "options.h"

#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <stb_image.h>

#define MAX_TILES	20000
#define MAX_VTX		(4 * MAX_TILES)
#define MAX_IDX		(6 * MAX_TILES)

typedef struct vertex_t vertex_t;
struct vertex_t
{
	float p[2];
	float t[2];
	float f[3];
	float b[3];
};

static struct {
	SDL_Window		*	window;
	SDL_GLContext 		context;
	SDL_Event			event;

	unsigned			tileset_id;
	unsigned			shader_id;
	unsigned			va, ib;

	vertex_t			v[MAX_VTX];
	unsigned short		i[MAX_IDX];
	unsigned			curr_v;
	unsigned 			curr_i;

	unsigned			display_x;
	unsigned			display_y;
	unsigned			tileset_x;
	unsigned			tileset_y;
	unsigned			window_x;
	unsigned			window_y;

	color_t				fore;
	color_t				back;
} terminal;

void gl_topen(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		error("SDL_Init(): %s", SDL_GetError());
	}

	int w, h, c;
	stbi_set_flip_vertically_on_load(1);
	unsigned char *buf = stbi_load(options_get("tileset"), &w, &h, &c, 0);
	if (buf == NULL) {
		error("stbi_load(): %s", stbi_failure_reason());
	}

	terminal.window_x = atoi(options_get("window_x"));
	terminal.window_y = atoi(options_get("window_y"));
	terminal.tileset_x = w;
	terminal.tileset_y = h;
	terminal.display_x = terminal.window_x * (terminal.tileset_x / 32);
	terminal.display_y = terminal.window_y * (terminal.tileset_y / 8);

	unsigned flags = atoi(options_get("windowed")) ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN_DESKTOP;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	terminal.window = SDL_CreateWindow("roguelike", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		terminal.display_x,
		terminal.display_y,
		flags | SDL_WINDOW_OPENGL);
	if (terminal.window == NULL) {
		error("SDL_CreateWindow(): %s", SDL_GetError());
	}

	terminal.context = SDL_GL_CreateContext(terminal.window);
	if (terminal.context == NULL) {
		error("SDL_GL_CreateContext(): %s", SDL_GetError());
	}

	glewExperimental = 1;
	int err = glewInit();
	if (err != GLEW_OK) {
		error("glewInit(): %s", glewGetErrorString(err));
	}

	terminal.curr_v = 0;
	terminal.curr_i = 0;

	glGenVertexArrays(1, &terminal.va);
	glBindVertexArray(terminal.va);

	unsigned vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * MAX_VTX, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 40, (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 40, (void*) 8);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 40, (void*)16);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 40, (void*)28);
	glEnableVertexAttribArray(3);

	static const unsigned short idx_pattern[] = {0, 1, 2, 2, 3, 0};
	for (int i = 0; i < MAX_IDX; i++) {
		terminal.i[i] = idx_pattern[i % 6] + ((i / 6) * 4);
	}

	glGenBuffers(1, &terminal.ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terminal.ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * MAX_IDX, terminal.i, GL_STATIC_DRAW);

	char *vsource;
	FILE *fp = fopen("shader.vs", "r");
	fseek(fp, 0, SEEK_END);
	int l = ftell(fp);
	rewind(fp);

	vsource = malloc(l + 1);
	fread(vsource, 1, l, fp);
	vsource[l] = '\0';
	fclose(fp);

	char *psource;
	fp = fopen("shader.ps", "r");
	fseek(fp, 0, SEEK_END);
	l = ftell(fp);
	rewind(fp);

	psource = malloc(l + 1);
	fread(psource, 1, l, fp);
	psource[l] = '\0';
	fclose(fp);

	int result;

	unsigned vshader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader_id, 1, (const char *const*)&vsource, NULL);
	glCompileShader(vshader_id);

	glGetShaderiv(vshader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		error("glCompileShader()");
	}

	unsigned pshader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pshader_id, 1, (const char *const*)&psource, NULL);
	glCompileShader(pshader_id);

	glGetShaderiv(pshader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		error("glCompileShader()");
	}

	terminal.shader_id = glCreateProgram();
	glAttachShader(terminal.shader_id, vshader_id);
	glAttachShader(terminal.shader_id, pshader_id);
	glLinkProgram(terminal.shader_id);
	free(psource);
	free(vsource);

	glGetProgramiv(terminal.shader_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		error("glLinkProgram()");
	}

	glDetachShader(terminal.shader_id, vshader_id);
	glDetachShader(terminal.shader_id, pshader_id);
	glDeleteShader(vshader_id);
	glDeleteShader(pshader_id);
	glUseProgram(terminal.shader_id);
	
	glGenTextures(1, &terminal.tileset_id);
	glBindTexture(GL_TEXTURE_2D, terminal.tileset_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, c == 3 ? GL_RGB8 : GL_RGBA8, 
			terminal.tileset_x, 
			terminal.tileset_y, 0, c == 3 ? GL_RGB : GL_RGBA, 
			GL_UNSIGNED_BYTE, buf);
	stbi_image_free(buf);

	static float ortho[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	ortho[0][0] = 2.0f / (terminal.window_x + 0.0f);
	ortho[1][1] = 2.0f / (terminal.window_y + 0.0f);
	ortho[2][2] = -1.0f;
	ortho[3][0] = -(terminal.window_x + 0.0f) / (terminal.window_x + 0.0f);
	ortho[3][1] = -(terminal.window_y + 0.0f) / (terminal.window_y + 0.0f);

	int loc = glGetUniformLocation(terminal.shader_id, "u_p");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &ortho[0][0]);
	glViewport(0, 0, terminal.display_x, terminal.display_y);

	palette_init();
	terminal.fore = C_WHITE;
	terminal.back = C_BLACK;

	tx = terminal.window_x;
	ty = terminal.window_y;

	static char title[128];
	snprintf(title, 128, "%ix%i", tx, ty);
	SDL_SetWindowTitle(terminal.window, title);
}

void gl_tclose(void)
{
	glDeleteTextures(1, &terminal.tileset_id);
	glDeleteProgram(terminal.shader_id);
	glDeleteBuffers(1, &terminal.ib);
	glDeleteVertexArrays(1, &terminal.va);
	SDL_DestroyWindow(terminal.window);
	SDL_Quit();
}

void gl_tclear(void)
{
	unsigned color = palette_get(terminal.back);
	unsigned char r = (color >> 16) & 0xFF;
	unsigned char g = (color >>  8) & 0xFF;
	unsigned char b = (color >>  0) & 0xFF;

	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	terminal.curr_v = 0;
	terminal.curr_i = 0;
}

void gl_tflush(void)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t) * terminal.curr_v, terminal.v);
	glDrawElements(GL_TRIANGLES, terminal.curr_i, GL_UNSIGNED_SHORT, 0);
	SDL_GL_SwapWindow(terminal.window);
}

void gl_tcolor(color_t fore, color_t back)
{
	terminal.fore = fore;
	terminal.back = back;
}

void gl_tputc(int x, int y, char c)
{	
	const int col = 32;
	const int row = 8;
	
	const int tile_w = terminal.tileset_x / col;
	const int tile_h = terminal.tileset_y / row;

	const int tile_x = (c & 0xFF) % col;
	const int tile_y = row - ((c & 0xFF) / col + 1);
	const float u1 = (tile_w * tile_x) / (float)(terminal.tileset_x);
	const float u2 = u1 + (tile_w / (float)(terminal.tileset_x));
	const float v1 = (tile_h * tile_y) / (float)(terminal.tileset_y);
	const float v2 = v1 + (tile_h / (float)(terminal.tileset_y));

	const float x1 = x;
	const float x2 = x1 + 1.0;
	const float y1 = terminal.window_y - (y + 1);
	const float y2 = y1 + 1.0;

	const unsigned f = palette_get(terminal.fore);
	const float fR = ((f >> 16) & 0xFF) / 255.0f;
	const float fG = ((f >>  8) & 0xFF) / 255.0f;
	const float fB = ((f >>  0) & 0xFF) / 255.0f;

	const unsigned b = palette_get(terminal.back);
	const float bR = ((b >> 16) & 0xFF) / 255.0f;
	const float bG = ((b >>  8) & 0xFF) / 255.0f;
	const float bB = ((b >>  0) & 0xFF) / 255.0f;

	terminal.v[terminal.curr_v++] = (vertex_t){.p = {x2, y2}, .t = {u2, v2}, .f = {fR, fG, fB}, .b = {bR, bG, bB}};
	terminal.v[terminal.curr_v++] = (vertex_t){.p = {x2, y1}, .t = {u2, v1}, .f = {fR, fG, fB}, .b = {bR, bG, bB}};
	terminal.v[terminal.curr_v++] = (vertex_t){.p = {x1, y1}, .t = {u1, v1}, .f = {fR, fG, fB}, .b = {bR, bG, bB}};
	terminal.v[terminal.curr_v++] = (vertex_t){.p = {x1, y2}, .t = {u1, v2}, .f = {fR, fG, fB}, .b = {bR, bG, bB}};
	terminal.curr_v %= MAX_VTX;
	terminal.curr_i += 6;
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

void gl_tputs(int x, int y, char *s)
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
				gl_tputc(x++, y, *s++);
			} else {
				gl_tcolor(ch_to_color(*s++), terminal.back);
			}
		} else {
			gl_tputc(x++, y, *s++);
		}
	}
}

void gl_tprintf(int x, int y, char *fmt, va_list args)
{
	static char s[256];
	vsnprintf(s, 255, fmt, args);

	gl_tputs(x, y, s);
}

// TODO: rewrite this
static void gl_tprintfs(int x, int y, char *fmt, ...)
{
	static char s[256];

	va_list args;
	va_start(args, fmt);
	vsnprintf(s, 255, fmt, args);
	va_end(args);

	gl_tputs(x, y, s);
}

void gl_tborder(int x, int y, int w, int h, char *t, char *b)
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

	gl_tputc(x    , y    , '\xDA');
	gl_tputc(x + w, y    , '\xBF');
	gl_tputc(x + w, y + h, '\xD9');
	gl_tputc(x    , y + h, '\xC0');
	for (int i = x + 1; i < x + w; i++) {
		gl_tputc(i, y    , '\xC4');
		gl_tputc(i, y + h, '\xC4');
	}

	for (int i = y + 1; i < y + h; i++) {
		gl_tputc(x    , i, '\xB3');
		gl_tputc(x + w, i, '\xB3');
	}

	for (int X = x + 1; X < x + w; X++) {
		for (int Y = y + 1; Y < y + h; Y++) {
			gl_tputc(X, Y, ' ');
		}
	}

	if (t) {
		const int l = strlen(t) + 4;
		gl_tprintfs(x + w / 2 - l / 2 + 1, y,
			"\xAE &Y%s&W \xAF", t);
	}

	if (b) {
		const int l = strlen(b) + 4;
		gl_tprintfs(x + w / 2 - l / 2, y + h, 
			"\xAE &Y%s&W \xAF", b);
	}
}

static int convert_keycode(int code)
{
	switch (code) {
		case SDLK_LEFT:		return K_LEFT;
		case SDLK_RIGHT:	return K_RIGHT;
		case SDLK_UP:		return K_UP;
		case SDLK_DOWN:		return K_DOWN;
		default:
			return K_ERR;
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

int gl_tgetc(void)
{
	while (SDL_PollEvent(&terminal.event)) {
		if (terminal.event.type == SDL_QUIT) {
			exit(EXIT_SUCCESS);
		} else if (terminal.event.type == SDL_KEYDOWN) {
			int key = terminal.event.key.keysym.sym;
			if (key > 0x80) {
				return convert_keycode(key);
			}

			return convert_unicode(key);
		}
	}

	return K_ERR;
}

