#include "config.h"

#include <string.h>
#include <stdlib.h>
// #include <stdio.h>
#include <ctype.h>

static int ln;

static void config_error(char *m, char *p)
{
	fprintf(stderr, "\e[31merror:\e[m %s\n", m);
	fprintf(stderr, "\t[%i]: %s", ln, p);
}

// TODO: add comments, add multiple values
static void parse_line(config_t *cfg, char *p)
{
	++ln;

	char *q = p;

	char k[128];
	char v[128];

	while (*q) {
		if (*q == '#') {	// comment
			return;
		}

		if (isalpha(*q) || *q == '_') {
			char *a = q;
			while (isalpha(*q) || *q == '_') {
				if (!*q) {
					config_error("unexpected end of line", p);
					return;
				}

				q++;
			}

			strlcpy(k, a, (q - a) + 1);
		}

		if (*q == '[') {
			char *a = ++q;
			while (*q != ']') {
				if (!*q) {
					config_error("unexpected end of line", p);
					return;
				}

				q++;
			}

			strlcpy(v, a, (q - a) + 1);
		}

		while (isspace(*q)) {
			q++;
		}

		q++;
	}

	cfg->items = realloc(cfg->items, (cfg->size + 1) * sizeof(*cfg->items));
	cfg->items[cfg->size].k = strdup(k);
	cfg->items[cfg->size].v = strdup(v);
	cfg->size++;
}

void config_load(config_t *cfg, FILE *fp)
{
	ln = 0;
	static char line[256];
	while (fgets(line, 256, fp) != NULL) {
		parse_line(cfg, line);
		memset(line, 0, 256);
	}
}

void config_free(config_t *cfg)
{
	for (unsigned i = 0; i < cfg->size; i++) {
		free(cfg->items[i].k);
		free(cfg->items[i].v);
	}

	free(cfg->items);
}

char *config_get(config_t *cfg, char *k)
{
	for (unsigned i = 0; i < cfg->size; i++) {
		if (strcmp(cfg->items[i].k, k) == 0)
			return cfg->items[i].v;
	}

	return NULL;
}

