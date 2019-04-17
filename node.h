#include<stdlib.h>
#include <ctype.h>
#include <string.h>



enum {
	ND_NUM = 256,
	ND_RETURN,
	ND_IDENT,
};

typedef struct Node {
	int ty;
	struct Node *lhs;
	struct Node *rhs;
	int val;
	char name;
} Node;


Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_ident(char name);
Node *new_node_num(int val);
//int consume(int ty);
int consume_vec(int ty);
Node *add();
Node *mul();
Node *term();
Node *term_vec();
Node *assign();
Node *stmt();
void program();
void gen_lval(Node *node);
void gen(Node *node);
