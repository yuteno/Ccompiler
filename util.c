#include "util.h"
#include "token.h"
extern Token tokens[100];

void error(int i)
{
	fprintf(stderr, "unexpected token: %s\n", tokens[i].input);
	exit(1);
}

