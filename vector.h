#ifndef H_VECTOR
#define H_VECTOR
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "util.h"

typedef struct {
	void **data;
	int capacity;
	int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void vec_test();
#endif
