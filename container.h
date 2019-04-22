#ifndef H_CONTAINER
#define H_CONTAINER
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "util.h"

typedef struct {
	void **data;
	int capacity;
	int len;
} Vector;

typedef struct {
	Vector *keys;
	Vector *vals;
} Map;


Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void vec_test();

Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
void map_test();
#endif
