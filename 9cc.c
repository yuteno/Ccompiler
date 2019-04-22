#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "node.h"
#include "token.h"
#include "util.h"
//#include "vector.h"
#include "map.h"


//extern Token tokens[100];
extern Vector *tokens_vec;
extern Vector *code_vec;
//extern int pos;







int main(int argc, char **argv)
{
	if (argc != 2){
		fprintf(stderr, "check num of argument");
		return 1;
	}

	if (strcmp(argv[1], "-test") == 0) {
		printf("now test start\n");
		vec_test();
		map_test();
		return 0;
	}
	//printf("now tokenize\n");
	tokenize_vec(argv[1]);
	program();
	//fprintf(stderr, "end program\n");
	//Node *node = add();

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	printf("	push rbp\n");
	printf("	mov rbp, rsp\n");
	printf("	sub rsp, %d\n", 8*26);

	//for (int i = 0; code[i]; i++) {
	for (int i = 0; (Node *)code_vec->data[i]; i++) {
		gen( (Node *)code_vec->data[i]);

	//gen(node);
		printf("	pop rax\n");
	}
	printf("	mov rsp, rbp\n");
	printf("	pop rbp\n");
	printf("	ret\n");
	return 0;
}




