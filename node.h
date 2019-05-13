#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "container.h"


enum {
	ND_NUM = 256,
	ND_EQUAL,
	ND_NEQUAL,
	ND_IF,
	ND_FOR,
	ND_WHILE,
	ND_RETURN, //262
	ND_IDENT, //263
	ND_BLOCK,
	ND_GEQUAL,
	ND_LEQUAL,
	ND_FUNCTION,
	ND_FUNCTION_DEF,
	ND_ARGUMENT,
};

typedef struct Node {
	int ty;
	struct Node *lhs;
	struct Node *rhs;
	struct Node *definition;
	struct Node *condition;
	struct Node *statement;
	struct Node *initialize;
	struct Node *control;
	Vector *block_contents;
	Vector *arguments;
	Map *local_variable_map;
	int *local_variable_count;
	int val;
	char *name;
	int arg_count;
} Node;


Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_ident(char *name);
Node *new_node_num(int val);
Node *new_node_function(char *name);
//Node *new_node_function(char *name, int ty);
Node *new_node_argument(int val, int arg_count);
//int consume(int ty);
int consume(int ty);
Node *add();
Node *mul();
Node *term();
Node *assign();
Node *stmt();
Node *unary();
Node *comp();
Node *block_items();
void program();
void gen_lval(Node *node);
void gen(Node *node);
void gen_definition(Node *node);
