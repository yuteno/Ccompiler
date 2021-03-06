#include "token.h"
//#include "vector.h"
//#include "map.h"
#include "container.h"
//Token tokens[100];
Vector *tokens_vec;
Map *variables;
int pos = 0;




void tokenize_vec(char *p)
{
	tokens_vec = new_vector();
	while (*p)
	{
		Token *token;
		token = malloc(sizeof(Token));
		if (isspace(*p))
		{
			p++;
			continue;
		}

		if (strncmp(p, "return", 6) == 0) {
			token->ty = TK_RETURN;
			token->input = p;
			p += 6;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, "if", 2) == 0) {
			token->ty = TK_IF;
			token->input = p;
			p += 2;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, "while", 5) == 0) {
			token->ty = TK_WHILE;
			token->input = p;
			p += 5;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, "for", 3) == 0) {
			token->ty = TK_FOR;
			token->input = p;
			p += 3;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, "==", 2) == 0) {
			token->ty = TK_EQUAL;
			token->input = p;
			p += 2;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, "!=", 2) == 0) {
			token->ty = TK_NEQUAL;
			token->input = p;
			p += 2;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, ">=", 2) == 0) {
			token->ty = TK_GEQUAL;
			token->input = p;
			p += 2;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (strncmp(p, "<=", 2) == 0) {
			token->ty = TK_LEQUAL;
			token->input = p;
			p += 2;
			vec_push(tokens_vec, (void *) token);
			continue;
		}


		if ('a' <= *p && *p <= 'z') {
			char *dup_p = strdup(p);
			//fprintf(stderr, "dup: %s\n", dup_p);
			char *tp = strtok(dup_p, " /+/-/*/\//(/)/=/;/,");
			//fprintf(stderr, "tp: %s\n", tp);


			//fprintf(stderr, "before: %s\n", p);
			while (('a' <= *p && *p <= 'z') || isdigit(*p)) {
				p++;
			}

			while (isspace(*p))
				p++;

			if (*p == '(') {
				token->ty = TK_FUNCTION;
				token->name = tp;
				vec_push(tokens_vec, (void *) token);
				p++;
				while (isspace(*p))
					p++;


				int arg_count = 0;
				//TODO 5/3
				//tokenize bug in arguments with + or - etc
				//for example func(num - 1) is tokenized func(num, -, 1);
				if (*p != ')') {
					//with arguments
					Token *argument = malloc(sizeof(Token));
					//fprintf(stderr, "call function: %s\n", tp);
					dup_p = strdup(p);
					dup_p = strtok(dup_p, ")");
					p += strlen(dup_p) + 1;
					//fprintf(stderr, "after strlen: %s\n", p);

					dup_p = strtok(dup_p,",");

					//fprintf(stderr, "argument1 %s\n", dup_p);
					if (isdigit(*dup_p)) {
						//fprintf(stderr, "digit OK %s\n");
						argument->ty = TK_ARGUMENT_NUM;
						argument->val = strtol(dup_p, &dup_p, 10);
					}
					else {
						//fprintf(stderr, "IDENT OK %s\n");
						argument->ty = TK_ARGUMENT_IDENT;
						argument->name = dup_p;
						//fprintf(stderr, "dup_p: %s\n", dup_p);
					}


					//fprintf(stderr, "argment1 val:%d\n", argument->val);
					argument->input = dup_p;
					argument->arg_count = arg_count;
					vec_push(tokens_vec, argument);
					arg_count++;


					while (dup_p != NULL) {
						dup_p = strtok(NULL, " ,");
						//fprintf(stderr, "dup_p:%s\n", dup_p);
						Token *argument1 = malloc(sizeof(Token));
						if (dup_p != NULL) {
							if (isdigit(*dup_p)) {
								argument1->ty = TK_ARGUMENT_NUM;
								argument1->val = strtol(dup_p, &dup_p, 10);
							} else {
								argument1->ty = TK_ARGUMENT_IDENT;
								argument1->name = dup_p;
							}
							argument1->input = dup_p;
							argument1->arg_count = arg_count;
							vec_push(tokens_vec, argument1);
							arg_count++;
							//fprintf(stderr, "argument1 push OK %s\n", dup_p);
						}
					}
				} else {
					//without arguments
					p++;
				}
				while(isspace(*p))
					p++;

				if (*p == '{') {
					((Token *)tokens_vec->data[tokens_vec->len-1 - arg_count])->ty = TK_FUNCTION_DEF;
					//fprintf(stderr, "after tokenize: %s\n", p);
				}

				//fprintf(stderr, "after tokenize: %s\n", p);
			}
			else {
				//fprintf(stderr, "tokenize: %s\n", tp);
				//fprintf(stderr, "after: %s\n", p);
				token->ty = TK_IDENT;
				token->name = tp;
				vec_push(tokens_vec, (void *) token);
			}
			continue;
		}

		if (*p == '+'
				|| *p == '-'
				|| *p == '*'
				|| *p == '/'
				|| *p == '('
				|| *p == ')'
				|| *p == '='
				|| *p == ';'
				|| *p == '<'
				|| *p == '>'
				|| *p == '&'
				|| *p == '|'
				|| *p == '{'
				|| *p == '}'
				)
		{
			token->ty = *p;
			token->input = p;
			p++;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if (isdigit(*p))
		{
			token->ty = TK_NUM;
			token->input = p;
			token->val = strtol(p, &p, 10);
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		fprintf(stderr, "can't tokenize: %s\n", p);
		exit(1);
	}
	Token *eof_token;
	eof_token = malloc(sizeof(Token));
	eof_token->ty = TK_EOF;
	eof_token->input = p;
	vec_push(tokens_vec, (void *)eof_token);
}


