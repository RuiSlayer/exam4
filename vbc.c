#include <stdio.h>
#include <stdlib.h>

char *g_expr;

int add(void);

void error(void)
{
	if (*g_expr == '\0')
		printf("Unexpected end of input\n");
	else
		printf("Unexpected token '%c'\n", *g_expr);
	exit(1);
}

int parent(void)
{
	int n;

	if (*g_expr >= '0' && *g_expr <= '9')
		return (*g_expr++ - '0');

	if (*g_expr == '(')
	{
		g_expr++;
		n = add();
		if (*g_expr != ')')
			error();
		g_expr++;
		return (n);
	}
	error();
return (0);
}

int multi(void)
{
	int n;

	n = parent();
	while (g_expr == '*')
	{
		g_expr++;
		n *= parent();
	}
	return (n);
}

int add(void)
{
	int n;

	n = multi();
	while (*g_expr == '+')
	{
		g_expr++;
		n += multi();
	}
	return (n);
}

int main(int argc, char **argv)
{
	int result;

	if (argc != 2)
		return (1);
	g_expr = argv[1];
	result = add();
	if (*g_expr != '\0')
		error();
	printf("%d\n", result);
}
