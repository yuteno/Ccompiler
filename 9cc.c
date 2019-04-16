#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "node.h"
#include "token.h"
#include "util.h"
#include "vector.h"



//extern Token tokens[100];
extern Vector *tokens_vec;
//extern int pos;


int expect(int line, int expected, int actual) {
	if (expected == actual)
		return;
	fprintf(stderr, "%d: %d expected, but got %d\n",
			line, expected, actual);
	exit(1);
}


void runtest() {
	Vector *vec = new_vector();
	expect(__LINE__, 0, vec->len);

	for (int i = 0; i < 100; i++)
		vec_push(vec, (void *)i);

	expect(__LINE__, 100, vec->len);
	expect(__LINE__, 0, (int)vec->data[0]);
	expect(__LINE__, 50, (int)vec->data[50]);
	expect(__LINE__, 99, (int)vec->data[99]);
	printf("Vector test OK\n");
}






int main(int argc, char **argv)
{
	if (argc != 2){
		fprintf(stderr, "check num of argument");
		return 1;
	}

	if (strcmp(argv[1], "-test") == 0) {
		printf("now test start\n");
		runtest();
		return 0;
	}
	//printf("now tokenize\n");
	tokenize_vec(argv[1]);
	Node *node = add();

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	gen(node);
	printf("	pop rax\n");
	printf("	ret\n");
	return 0;
}




