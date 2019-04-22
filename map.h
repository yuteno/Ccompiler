#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "vector.h"
#include "util.h"
typedef struct {
	Vector *keys;
	Vector *vals;
} Map;


Map *new_map();

void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
void map_test();

