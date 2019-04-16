#include<stdlib.h>
#include <ctype.h>
#include <string.h>



enum {
	ND_NUM = 256,
};

typedef struct Node {
	int ty;
	struct Node *lhs;
	struct Node *rhs;
	int val;
} Node;


Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
int consume(int ty);
Node *add();
Node *mul();
Node *term();
void gen(Node *node);
