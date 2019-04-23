#include<stdlib.h>
#include <ctype.h>
#include <string.h>



enum {
	ND_NUM = 256,
	ND_EQUAL,
	ND_NEQUAL,
	ND_RETURN,
	ND_IDENT,
	ND_GEQUAL,
	ND_LEQUAL,
	ND_FUNCTION,
	ND_ARGUMENT,
};

typedef struct Node {
	int ty;
	struct Node *lhs;
	struct Node *rhs;
	int val;
	char *name;
} Node;


Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_ident(char *name);
Node *new_node_num(int val);
Node *new_node_function(char *name);
//int consume(int ty);
int consume(int ty);
Node *add();
Node *mul();
Node *term();
Node *assign();
Node *stmt();
Node *unary();
Node *comp();
void program();
void gen_lval(Node *node);
void gen(Node *node);
