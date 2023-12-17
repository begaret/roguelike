#pragma once

typedef struct mdata_t mdata_t;
struct mdata_t
{
	char *id;

	unsigned color1 : 4;
	unsigned color2 : 4;
	char tile;
	unsigned flags;
};

typedef struct tdata_t tdata_t;
struct tdata_t
{
	char *id;

	unsigned color1 : 4;
	unsigned color2 : 4;
	char tile;
	unsigned flags;
};

void data_init(void);

void data_exit(void);

mdata_t *data_get_mdata(char *id);

tdata_t *data_get_tdata(char *id);

