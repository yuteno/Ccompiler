#include "node.h"
#include "token.h"
#include "util.h"
//#include "vector.h"
//#include "map.h"
#include "container.h"

//extern Token tokens[100];
extern Vector *tokens_vec;
extern int pos;
Vector *code_vec;
Map *variable_map;
int variable_count = 0;
int loop_count = 0;

void program() {
	code_vec = new_vector();
	variable_map = new_map();
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

}


int consume(int ty) {
	if ( ((Token *)(tokens_vec->data[pos]))->ty != ty )
		return 0;
	pos++;
	return 1;
}

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
	//fprintf(stderr, "ident name: %s\n", name);
	if (map_get(variable_map, name) == NULL) {
		variable_count++;
		map_put(variable_map, name, variable_count);
		//fprintf(stderr, "ident name: %s variable_count:%d\n", name, variable_count);
	}
	return node;
}


Node *new_node_function(char *name) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_FUNCTION;
	node->name = name;
	//fprintf(stderr, "ident name: %s\n", name);
	/*
	if (map_get(variable_map, name) == NULL) {
		variable_count++;
		map_put(variable_map, name, variable_count);
		//fprintf(stderr, "ident name: %s variable_count:%d\n", name, variable_count);
	}
	*/
	return node;
}

Node *new_node_argument(int val, int arg_count) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_ARGUMENT;
	node->val = val;
	node->arg_count = arg_count;
	fprintf(stderr, "argument node val:%d\n", val);
	return node;
}

Node *assign() {
	Node *node = comp();
	while (consume('='))
		node = new_node('=', node, assign());
	return node;
}


Node *stmt() {
	Node *node;
	if (consume('{')) {
		//fprintf(stderr, "now start block %s", ((Token *) tokens_vec->data[pos])->input);
		//Node *node = add();
		Node *node = block_items();
		//fprintf(stderr, "now end  block %s", ((Token *) tokens_vec->data[pos])->input);
		if (!consume('}'))
			fprintf(stderr, "close brace should come after open brace: %s\n", ((Token *) tokens_vec->data[pos])->input);
		return node;
	}
	if (consume(TK_RETURN)) {
		node = malloc(sizeof(Node));
		node->ty = ND_RETURN;
		node->lhs = assign();
	} else if (consume(TK_IF)) {
		node = malloc(sizeof(Node));
		node->ty = ND_IF;
		node->condition = assign();
		node->statement = stmt();
		return node;
	} else if (consume(TK_WHILE)) {
		node = malloc(sizeof(Node));
		node->ty = ND_WHILE;
		node->condition = assign();
		node->statement = stmt();
		return node;
	} else if (consume(TK_FOR)) {
		node = malloc(sizeof(Node));
		node->ty = ND_FOR;
		while (!consume('('))
			pos++;

		node->initialize = stmt();
		node->condition = stmt();
		node->control = assign();
		while (!consume(')'))
			pos++;
		node->statement = stmt();
		return node;
	} else {
		node = assign();
	}


	if (!consume(';')) {
		fprintf(stderr, "this token is not ';': %s\n", ((Token *)(tokens_vec->data[pos]))->input);
	}
	return node;
}

Node *block_items() {
	Node *node;
	node = malloc(sizeof(Node));
	node->ty = ND_BLOCK;
	node->block_contents = new_vector();
	while (!consume('}')) {
		Node *new_node = stmt();
		vec_push(node->block_contents, new_node);
	}
	pos--;
	return node;
}




Node *comp() {
	Node *node = add();
	for (;;) {
		if (consume(TK_EQUAL))
			node = new_node(ND_EQUAL, node, add());
		else if (consume(TK_NEQUAL))
			node = new_node(ND_NEQUAL, node, add());
		else if (consume(TK_GEQUAL))
			node = new_node(ND_GEQUAL, node, add());
		else if (consume(TK_LEQUAL))
			node = new_node(ND_LEQUAL, node, add());
		else if (consume('<'))
			node = new_node('<', node, add());
		else if (consume('>'))
			node = new_node('>', node, add());
		else
			return node;
	}
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
	Node *node = unary();

	for (;;) {
		if (consume('*'))
			node = new_node('*', node, unary());
		else if (consume('/'))
			node = new_node('/', node, unary());
		else
			return node;
	}
}


Node *unary() {
	if (consume('+'))
		return term();
	if (consume('-'))
		return new_node('-', new_node_num(0), term());
	return term();
}

Node *term() {
	if (consume('(')) {
		//Node *node = add();
		Node *node = assign();
		if (!consume(')'))
			fprintf(stderr, "close bracket should come after open bracket: %s", ((Token *) tokens_vec->data[pos])->input);
		return node;
	}

	if (((Token *) tokens_vec->data[pos])->ty == TK_NUM)
		return new_node_num(((Token *) tokens_vec->data[pos++])->val);

	if (((Token *) tokens_vec->data[pos])->ty == TK_IDENT)
		return new_node_ident(((Token *) tokens_vec->data[pos++])->name);
	//return new_node_ident( *(((Token *) tokens_vec->data[pos++])->input));
	if (((Token *) tokens_vec->data[pos])->ty == TK_FUNCTION)
		return new_node_function(((Token *) tokens_vec->data[pos++])->name);

	if (((Token *) tokens_vec->data[pos])->ty == TK_ARGUMENT)
		return new_node_argument(((Token *) tokens_vec->data[pos++])->val, ((Token *) tokens_vec->data[pos++])->arg_count);


	fprintf(stderr, "This token is not NUM or open bracket: %s\n", ((Token *) tokens_vec->data[pos])->input);
}


void gen_lval(Node *node) {
	if (node->ty != ND_IDENT)
		fprintf(stderr, "left value is not variable\n");

	//int offset = ('z' - node->name[0] + 1) * 8;
	int offset =( (int) map_get(variable_map, node->name) + 1) * 8;
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

	if (node->ty == ND_IF) {
		gen(node->condition);
		printf("	pop rax\n");
		printf("	cmp rax, 0\n");
		printf("	je .Lend%04d\n", loop_count);
		gen(node->statement);
		printf(".Lend%04d:\n", loop_count);
		loop_count++;
		return;
	}

	if (node->ty == ND_WHILE) {
		printf(".Lbegin%04d:\n", loop_count);
		gen(node->condition);
		printf("	pop rax\n");
		printf("	cmp rax, 0\n");
		printf("	je .Lend%04d\n", loop_count);
		gen(node->statement);
		printf("	jmp .Lbegin%04d\n", loop_count);
		printf(".Lend%04d:\n", loop_count);
		loop_count++;
		return;
	}

	if (node->ty == ND_FOR) {
		gen(node->initialize);
		printf(".Lbegin%04d:\n", loop_count);
		gen(node->condition);
		printf("	pop rax\n");
		printf("	cmp rax, 0\n");
		printf("	je .Lend%04d\n", loop_count);
		gen(node->statement);
		gen(node->control);
		printf("	jmp .Lbegin%04d\n", loop_count);
		printf(".Lend%04d:\n", loop_count);
		loop_count++;
		return;
	}

	if (node->ty == ND_BLOCK) {
		int i_;
		for (i_ = 0; i_ < node->block_contents->len; i_++)
			gen(node->block_contents->data[i_]);
		return;
	}

	if (node->ty == ND_NUM) {
		//fprintf(stderr, "push %d\n", node->val);
		printf("	push %d\n", node->val);
		return;
	}

	if (node->ty == ND_ARGUMENT) {
		char *reg_name = malloc(sizeof(char) * 10);
		switch (node->arg_count) {
			case 0:
				reg_name = "rdi";
				break;
			case 1:
				reg_name = "rsi";
				break;
			case 2:
				reg_name = "rdx";
				break;
			case 3:
				reg_name = "rcx";
				break;
			case 4:
				reg_name = "r8";
				break;
			case 5:
				reg_name = "r9";
				break;
			default:
				//TODO support over seven arguments
				break;
		}

		//fprintf(stderr, "mov rbp %d\n", node->val);
		//printf("	push rbp %d\n", node->val);
		printf("	mov %s, %d\n", reg_name, node->val);
		return;
	}

	if (node->ty == ND_FUNCTION) {
		printf("	call %s\n", node->name);
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
			break;
		case '>':
			printf("	cmp rdi, rax\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;
		case '<':
			printf("	cmp rax, rdi\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_GEQUAL:
			printf("	cmp rdi, rax\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_LEQUAL:
			printf("	cmp rax, rdi\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_EQUAL:
			printf("	cmp rdi, rax\n");
			printf("	sete al\n");
			printf("	movzb rax, al\n");
			break;
		case ND_NEQUAL:
			printf("	cmp rdi, rax\n");
			printf("	setne al\n");
			printf("	movzb rax, al\n");
	}
	printf("	push rax\n");
}

