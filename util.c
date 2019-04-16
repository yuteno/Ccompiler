#include "util.h"
#include "token.h"
#include "vector.h"
//extern Token tokens[100];
extern Vector *tokens_vec;

void error(int i)
{
	fprintf(stderr, "unexpected token: %s\n", ((Token *)tokens_vec->data[i])->input);
	exit(1);
}

