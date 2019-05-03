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


int buzz3 (int x, int y, int z) {
	printf("function buzz3 is called, result: %d\n", x + y + z);
}

int sum_10(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9, int x10) {
	printf("function sum_10 is called, result: %d\n",
			x1 +
			x2 +
			x3 +
			x4 +
			x5 +
			x6 +
			x7 +
			x8 +
			x9 +
			x10
			);
}
