#include<stdio.h>

int foo (){
	printf("function foo OK\n");
}

int bar (){
	printf("function bar is called\n");
}


int buzz (int x, int y) {
	printf("function buzz is called, result: %d\n", x + y);
}

