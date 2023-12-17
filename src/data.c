#include "data.h"

#include "palette.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct data_item_t data_item_t;
struct data_item_t
{
	union
	{
		tdata_t t;
		mdata_t m;
	};

	data_item_t *next;
};

typedef struct data_t data_t;
struct data_t
{
	data_item_t *m;
	data_item_t *t;
};

static data_t data = {0};

void data_init(void)
{
	data_item_t *m = malloc(sizeof(*m));
	m->m = (mdata_t){
		"m_human",
		C_WHITE,
		C_BLACK,
		'@',
		0
	};
	m->next = NULL;
	data.m = m;

	data_item_t *t = malloc(sizeof(*t));
	t->t = (tdata_t){
		"t_grass",
		C_LGREEN,
		C_BLACK,
		',',
		0
	};
	t->next = NULL;
	data.t = t;
	t = malloc(sizeof(*t));
	t->t = (tdata_t){
		"t_bush",
		C_GREEN,
		C_BLACK,
		'#',
		0
	};
	t->next = data.t;
	data.t = t;
}

void data_exit(void)
{
	
}

mdata_t *data_get_mdata(char *id)
{
	data_item_t *d = data.m;
	while (d) {
		if (strcmp(id, d->m.id) == 0)
			return &d->m;

		d = d->next;
	}

	return NULL;
}

tdata_t *data_get_tdata(char *id)
{	
	data_item_t *d = data.t;
	while (d) {
		if (strcmp(id, d->t.id) == 0)
			return &d->t;

		d = d->next;
	}

	return NULL;
}

