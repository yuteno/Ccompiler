#include "token.h"


Token tokens[100];
int pos = 0;

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
