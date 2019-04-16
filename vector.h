#include<stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
	void **data;
	int capacity;
	int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
