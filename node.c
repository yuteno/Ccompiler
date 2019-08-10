#include "node.h"
#include "token.h"
#include "util.h"
//#include "vector.h"
//#include "map.h"
#include "container.h"



extern Vector *tokens_vec;
extern int pos;
Vector *code_vec;
Vector *func_definitions;
Map *variable_map;
int *variable_count;
int loop_count = 0;

void program() {
	code_vec = new_vector();
	func_definitions = new_vector();
	//variable_map = new_map();
	int i = 0;
	while (((Token *)(tokens_vec->data[pos]))->ty != TK_EOF) {
		Node *_code;
		_code = malloc(sizeof(Node));
		if (((Token *)(tokens_vec->data[pos]))->ty != TK_FUNCTION_DEF) {
			_code = stmt();
			vec_push(code_vec, (void *) _code);
		} else {
			_code = stmt();
			vec_push(func_definitions, (void *) _code);
		}
	}
	Node *null_code;
	null_code = NULL;
	vec_push(code_vec, (void *) null_code);
	vec_push(func_definitions, (void *) null_code);

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
		*variable_count += 1;
		map_put(variable_map, name, *variable_count);
		//fprintf(stderr, "ident name: %s variable_count:%d\n", name, variable_count);
	}
	return node;
}


Node *new_node_function(char *name) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_FUNCTION;
	node->name = name;
	node->arguments = new_vector();
	//fprintf(stderr, "new_node_func -1 ty:%d\n", ((Token *)tokens_vec->data[pos-1])->ty);
	//fprintf(stderr, "new_node_func ty:%d\n", ((Token *)tokens_vec->data[pos])->ty);
	int arg_count = 0;
	while (((Token *) tokens_vec->data[pos])->ty == TK_ARGUMENT_NUM || ((Token *) tokens_vec->data[pos])->ty == TK_ARGUMENT_IDENT) {
		Node *argument = malloc(sizeof(Node));
		if (((Token *) tokens_vec->data[pos])->ty == TK_ARGUMENT_NUM)
			argument = new_node_num(((Token *) tokens_vec->data[pos++])->val);

		if (((Token *) tokens_vec->data[pos])->ty == TK_ARGUMENT_IDENT) {
			//fprintf(stderr, "new_node_func ty == TK_ARGUMENT_IDENT\n");
			argument = new_node_ident(((Token *) tokens_vec->data[pos++])->name);
			//fprintf(stderr, "end TK_ARGUMENT_IDENT\n");
		}
		arg_count++;
		vec_push(node->arguments, argument);

	}
	node->arg_count = arg_count;

	return node;
}

Node *new_node_function_def(char *name) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_FUNCTION_DEF;
	node->name = name;
	node->arguments = new_vector();
	node->local_variable_map = new_map();
	node->local_variable_count = malloc(sizeof(int));
	*(node->local_variable_count) = 0;
	//fprintf(stderr, "fucntion name %s map renew\n", node->name);
	variable_map = node->local_variable_map;
	variable_count = node->local_variable_count;
	//fprintf(stderr, "new_node_func -1 ty:%d\n", ((Token *)tokens_vec->data[pos-1])->ty);
	//fprintf(stderr, "new_node_func ty:%d\n", ((Token *)tokens_vec->data[pos])->ty);
	int arg_count = 0;
	while (((Token *) tokens_vec->data[pos])->ty == TK_ARGUMENT_IDENT) {
		Node *argument = malloc(sizeof(Node));
		//fprintf(stderr, "new_node_func ty == TK_ARGUMENT_IDENT\n");
		argument = new_node_ident(((Token *) tokens_vec->data[pos++])->name);
		//fprintf(stderr, "end TK_ARGUMENT_IDENT\n");
		vec_push(node->arguments, argument);
		arg_count++;
	}
	node->arg_count = arg_count;
	node->definition = stmt();
	//fprintf(stderr, "function %s map len %d\n", node->name, variable_map->keys->len);

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

	} else if (((Token *)(tokens_vec->data[pos]))->ty == TK_FUNCTION_DEF) {
		node = new_node_function_def(((Token *) tokens_vec->data[pos++])->name);
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

	 //TODO
	if ((consume('*'))){
		Node *node = malloc(sizeof(Node));
		node->ty = ND_DEREF;
		node->lhs = unary();
		return node;
	}

	if ((consume('&'))){
		Node *node = malloc(sizeof(Node));
		node->ty = ND_ADDR;
		node->lhs = unary();
		return node;
	}

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

	if (((Token *) tokens_vec->data[pos])->ty == TK_FUNCTION)
		return new_node_function(((Token *) tokens_vec->data[pos++])->name);


	fprintf(stderr, "This token is not NUM or open bracket: %s\n", ((Token *) tokens_vec->data[pos])->input);
}


void gen_definition(Node *node) {
	variable_map = node->local_variable_map;
	variable_count = node->local_variable_count;
	//fprintf(stderr, "generating input definition %s \n", node->name);
	printf("\n");
	printf("%s:\n", node->name);
	//TODO argument supporting
	printf("	push rbp\n");
	printf("	mov rbp, rsp\n");
	printf("	sub rsp, %d\n", 32*26);
	if (strncmp(node->name, "main", 4) == 0) {
	} else {
		//printf("	push rbp\n");
		//	printf("	mov rbp, rsp\n");
		//	printf("	sub rsp, %d\n", node->arg_count * 8);
		for (int i = 0; i < node->arg_count; i++) {
			char *reg_name = malloc(sizeof(char) * 10);
			switch (i) {
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
			printf("	mov [rbp-%d], %s\n", (i + 1) * 32, reg_name);
		}
	}

	gen(node->definition);
}


void gen_lval(Node *node) {
	//fprintf(stderr, "generating lval input %s \n", node->name);
	if (node->ty != ND_IDENT)
		fprintf(stderr, "left value is not variable\n");

	//int offset = ('z' - node->name[0] + 1) * 8;
	int offset =( (int) map_get(variable_map, node->name)) * 32;
	fprintf(stderr, "node name %s offset %d\n", node->name, offset);
	printf("	mov rax, rbp\n");
	printf("	sub rax, %d\n", offset);
	printf("	push rax\n");
}


void gen(Node *node) {
	//fprintf(stderr, "generating input %d \n", node->ty);


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
		printf("	push %d\n", node->val);
		return;
	}


	if (node->ty == ND_FUNCTION) {
		int arg_count;
		for (arg_count = 0; arg_count < node->arguments->len; arg_count++) {
			gen(node->arguments->data[arg_count]);
			//TODO set RSP to multiple of 16?
			//fprintf(stderr, "argument %d\n", ((Node *)node->arguments->data[arg_count])->val);

		}

		for (arg_count = node->arguments->len - 1; arg_count >= 0; arg_count--) {
			char *reg_name = malloc(sizeof(char) * 10);
			switch (arg_count) {
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

			printf("	pop %s\n", reg_name);
		}
		printf("	call %s\n", node->name);
		//TODO? next line is needed?
		printf("	push rax\n");
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

	if (node->ty == ND_ADDR) {
		gen_lval(node->lhs);
		return;
	}

	if (node->ty == ND_DEREF) {
		gen(node->lhs);
		printf("	pop rax\n");
		printf("	mov rax, [rax]\n");
		printf("	push rax\n");
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

