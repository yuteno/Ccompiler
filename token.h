#ifndef H_TOKEN
#define H_TOKEN
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "container.h"


enum {
	TK_NUM = 256,
	TK_IDENT,
	TK_EQUAL,
	TK_NEQUAL,
	TK_GEQUAL,
	TK_LEQUAL,
	TK_RETURN,
	TK_FUNCTION,
	TK_ARGUMENT,
	TK_EOF,
};


typedef struct {
	int ty;
	int val;
	char *input;
	char *name;
	int arg_count;
} Token;

//extern Token tokens[100];

void tokenize(char *p);
void tokenize_vec(char *p);
#endif
