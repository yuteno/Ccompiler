#include "token.h"
#include "vector.h"
#include "map.h"
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

		if (*p == '+'
				|| *p == '-'
				|| *p == '*'
				|| *p == '/'
				|| *p == '('
				|| *p == ')'
				|| *p == '='
				|| *p == ';'
		   )
		{
			token->ty = *p;
			token->input = p;
			p++;
			vec_push(tokens_vec, (void *) token);
			continue;
		}

		if ('a' <= *p && *p <= 'z') {
			int variable_len = 0;
			char *dup_p = strdup(p);
			//fprintf(stderr, "dup: %s\n", dup_p);
			char *tp = strtok(dup_p, " /+/-/*/\//(/)/=/;");
			//fprintf(stderr, "tp: %s\n", tp);


			//fprintf(stderr, "before: %s\n", p);
			while(('a' <= *p && *p <= 'z') || isdigit(*p)) {
				p++;
			}


			//fprintf(stderr, "tokenize: %s\n", tp);
			//fprintf(stderr, "after: %s\n", p);

			token->ty = TK_IDENT;
			token->name = tp;
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

