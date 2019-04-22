#include "token.h"
#include "vector.h"
#include "map.h"
//Token tokens[100];
Vector *tokens_vec;
Map *variables;
int pos = 0;
/*
void tokenize(char *p)
{
	int i = 0;
	while (*p)
	{
		if (isspace(*p))
		{
			p++;
			continue;
		}

		if (*p == '+'
				|| *p == '-'
				|| *p == '*'
				|| *p == '/'
				|| *p == '('
				|| *p == ')'
				)
		{
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}

		if (isdigit(*p))
		{
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p, &p, 10);
			i++;
			continue;
		}

		fprintf(stderr, "can't tokenize: %s\n", p);
		exit(1);
	}

	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
}
*/


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
			for (int i = 0;;i++) {
				if (p[i] == '+'
						|| p[i] == '-'
						|| p[i] == '*'
						|| p[i] == '/'
						|| p[i] == '('
						|| p[i] == ')'
						|| p[i] == '='
						|| p[i] == ';'
				   )
				{
					break;
				}
				variable_len++;
			}
			char* temp_token = (char *)malloc(variable_len * sizeof(char));
			strncpy(temp_token, p, variable_len);

			fprintf(stderr, "before: %s\n", p);
			fprintf(stderr, "tokenize: %s\n", temp_token);
			p += variable_len;
			fprintf(stderr, "after: %s\n", p);

			token->ty = TK_IDENT;
			//token->input = p;
			token->input = temp_token;
			//p++;
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

