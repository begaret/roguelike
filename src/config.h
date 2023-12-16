#pragma once

#include <stdio.h>

typedef struct config_t config_t;
struct config_t
{
	unsigned size;
	struct {
		char *k;
		char *v;
	} *items;
};

void config_load(config_t *cfg, FILE *fp);

void config_free(config_t *cfg);

char *config_get(config_t *cfg, char *k);

