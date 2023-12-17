#include "data.h"

#include "palette.h"

#include <stdlib.h>
#include <string.h>

typedef struct data_t data_t;
struct data_t
{
	unsigned mmax;
	unsigned mlen;
	mdata_t *m;

	unsigned tmax;
	unsigned tlen;
	tdata_t *t;
};

static data_t data = {0};

void data_init(void)
{
	data.mmax = 8;
	data.mlen = 0;
	data.m = malloc(data.mmax * sizeof(*data.m));

	data.tmax = 8;
	data.tlen = 0;
	data.t = malloc(data.tmax * sizeof(*data.t));
}

void data_exit(void)
{
	for (unsigned i = 0; i < data.mlen; i++) {
		free(data.m[i].id);
	}

	free(data.m);
	for (unsigned i = 0; i < data.tlen; i++) {
		free(data.t[i].id);
	}

	free(data.t);
}

void data_add_mdata(mdata_t m)
{
	if (data.mlen == data.mmax) {
		data.m = realloc(data.m, data.mmax *= 2);
	}

	data.m[data.mlen++] = m;
}

void data_add_tdata(tdata_t t)
{
	if (data.tlen == data.tmax) {
		data.t = realloc(data.t, data.tmax *= 2);
	}

	data.t[data.tlen++] = t;
}
mdata_t *data_get_mdata(char *id)
{
	for (unsigned i = 0; i < data.mlen; i++) {
		if (strcmp(id, data.m[i].id) == 0)
			return &data.m[i];
	}

	return NULL;
}

tdata_t *data_get_tdata(char *id)
{	
	for (unsigned i = 0; i < data.tlen; i++) {
		if (strcmp(id, data.t[i].id) == 0)
			return &data.t[i];
	}

	return NULL;
}

