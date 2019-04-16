#include "node.h"
#include "token.h"
#include "util.h"

extern Token tokens[100];
extern int pos;


Node *new_node(int ty, Node *lhs, Node *rhs) {
	Node *node = malloc(sizeof(Node));
	node->ty = ty;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}


Node *new_node_num(int val) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_NUM;
	node->val = val;
	return node;
}


int consume(int ty) {
	if (tokens[pos].ty != ty)
		return 0;
	pos++;
	return 1;
}


Node *add() {
	Node *node = mul();
	for (;;) {
		if (consume('+'))
			node = new_node('+', node, mul());
		else if (consume('-'))
			node = new_node('-', node, mul());
		else
			return node;
	}
}

Node *mul() {
	Node *node = term();

	for (;;) {
		if (consume('*'))
			node = new_node('*', node, term());
		else if (consume('/'))
			node = new_node('/', node, term());
		else
			return node;
	}
}



Node *term() {
	if (consume('(')) {
		Node *node = add();
		if (!consume(')'))
			fprintf(stderr, "close bracket should come after open bracket: %s", tokens[pos].input);
		return node;
	}

	if (tokens[pos].ty == TK_NUM)
		return new_node_num(tokens[pos++].val);

	fprintf(stderr, "This token is not NUM or open bracket: %s", tokens[pos].input);
}



void gen(Node *node) {
	if (node->ty == ND_NUM) {
		printf("	push %d\n", node->val);
		return;
	}

	gen(node->lhs);
	gen(node->rhs);

	printf("	pop rdi\n");
	printf("	pop rax\n");


	switch (node->ty) {
	case '+':
		printf("	add rax, rdi\n");
		break;
	case '-':
		printf("	sub rax, rdi\n");
		break;
	case '*':
		printf("	mul rdi\n");
		break;
	case '/':
		printf("	mov rdx, 0\n");
		printf("	div rdi\n");
	}
	printf("	push rax\n");
}


