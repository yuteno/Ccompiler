#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "node.h"
#include "token.h"
#include "util.h"



extern Token tokens[100];
//extern int pos;




int main(int argc, char **argv)
{
	if (argc != 2){
		fprintf(stderr, "check num of argument");
		return 1;
	}
	tokenize(argv[1]);
	Node *node = add();

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	/*
	if (tokens[0].ty != TK_NUM) {
		error(0);
	}
	printf("	mov rax, %d\n", tokens[0].val);

	int i = 1;
	while (tokens[i].ty != TK_EOF)
	{
		if (tokens[i].ty == '+') {
			i++;
			if (tokens[i].ty != TK_NUM)
				error(i);

			printf("	add rax, %d\n", tokens[i].val);
			i++;
			continue;
		}

		if (tokens[i].ty == '-') {
			i++;
			if (tokens[i].ty != TK_NUM)
				error(i);
			printf("	sub rax, %d\n", tokens[i].val);
			i++;
			continue;
		}

		error(i);
	}

	printf("	ret\n");
	*/
	gen(node);
	printf("	pop rax\n");
	printf("	ret\n");
	return 0;
}




