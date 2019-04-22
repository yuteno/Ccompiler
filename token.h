#ifndef H_TOKEN
#define H_TOKEN
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "map.h"


enum {
	TK_NUM = 256,
	TK_IDENT,
	TK_RETURN,
	TK_EOF,
};


typedef struct {
	int ty;
	int val;
	char *input;
	char *name;
} Token;

//extern Token tokens[100];

void tokenize(char *p);
void tokenize_vec(char *p);
#endif
