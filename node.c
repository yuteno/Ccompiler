#include "node.h"
#include "token.h"
#include "util.h"
#include "vector.h"

//extern Token tokens[100];
extern Vector *tokens_vec;
extern int pos;
Vector *code_vec;


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

Node *new_node_ident(char *name) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_IDENT;
	node->name = name;
	return node;
}


Node *assign() {
	Node *node = add();
	while (consume_vec('='))
		node = new_node('=', node, assign());
	return node;
}


Node *stmt() {
	Node *node;
	if (consume_vec(TK_RETURN)) {
		node = malloc(sizeof(Node));
		node->ty = ND_RETURN;
		node->lhs = assign();
	} else {
		node = assign();
	}


	if (!consume_vec(';'))
		fprintf(stderr, "this token is not ';': %s", ((Token *)(tokens_vec->data[pos]))->input);
	return node;
}

void program() {
	code_vec = new_vector();
	int i = 0;
	while (((Token *)(tokens_vec->data[pos]))->ty != TK_EOF) {
		Node *_code;
		_code = malloc(sizeof(Node));
		//code[i++] = stmt();
		_code = stmt();
		vec_push(code_vec, (void *) _code);
	}
	Node *null_code;
	null_code = NULL;
	vec_push(code_vec, (void *) null_code);

	//code[i] = NULL;
}


int consume_vec(int ty) {
	if ( ((Token *)(tokens_vec->data[pos]))->ty != ty )
		return 0;
	pos++;
	return 1;
}

Node *add() {
	Node *node = mul();
	for (;;) {
		if (consume_vec('+'))
			node = new_node('+', node, mul());
		else if (consume_vec('-'))
			node = new_node('-', node, mul());
		else
			return node;
	}
}

Node *mul() {
	Node *node = term_vec();

	for (;;) {
		if (consume_vec('*'))
			node = new_node('*', node, term_vec());
		else if (consume_vec('/'))
			node = new_node('/', node, term_vec());
		else
			return node;
	}
}



Node *term_vec() {
	if (consume_vec('(')) {
		//Node *node = add();
		Node *node = assign();
		if (!consume_vec(')'))
			fprintf(stderr, "close bracket should come after open bracket: %s", ((Token *) tokens_vec->data[pos])->input);
		return node;
	}

	if (((Token *) tokens_vec->data[pos])->ty == TK_NUM)
		return new_node_num(((Token *) tokens_vec->data[pos++])->val);


	if (((Token *) tokens_vec->data[pos])->ty == TK_IDENT)
		return new_node_ident( *(((Token *) tokens_vec->data[pos++])->input));


	fprintf(stderr, "This token is not NUM or open bracket: %s", ((Token *) tokens_vec->data[pos])->input);
}


void gen_lval(Node *node) {
	if (node->ty != ND_IDENT)
		fprintf(stderr, "left value is not variable");

	int offset = ('z' - node->name[0] + 1) * 8;
	printf("	mov rax, rbp\n");
	printf("	sub rax, %d\n", offset);
	printf("	push rax\n");
}


void gen(Node *node) {
	if (node->ty == ND_RETURN) {
		gen(node->lhs);
		printf("	pop rax\n");
		printf("	mov rsp, rbp\n");
		printf("	pop rbp\n");
		printf("	ret\n");
		return;
	}

	if (node->ty == ND_NUM) {
		printf("	push %d\n", node->val);
		return;
	}

	if (node->ty == ND_IDENT) {
		gen_lval(node);
		printf("	pop rax\n");
		printf("	mov rax, [rax]\n");
		printf("	push rax\n");
		return;
	}

	if (node->ty == '=') {
		gen_lval(node->lhs);
		gen(node->rhs);

		printf("	pop rdi\n");
		printf("	pop rax\n");
		printf("	mov [rax], rdi\n");
		printf("	push rdi\n");
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


