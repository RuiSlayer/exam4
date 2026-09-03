#include <stdio.h>
#include <stdlib.h>

char *g_expr;

int expr(void);

void error(void)
{
    if (*g_expr == '\0')
        printf("Unexpected end of input\n");
    else
        printf("Unexpected token '%c'\n", *g_expr);
    exit(1);
}

int factor(void)
{
    int n;

    if (*g_expr >= '0' && *g_expr <= '9')
        return (*g_expr++ - '0');

    if (*g_expr == '(')
    {
        g_expr++;
        n = expr();
        if (*g_expr != ')')
            error();
        g_expr++;
        return (n);
    }
    error();
    return (0);
}

int term(void)
{
    int n;

    n = factor();
    while (g_expr == '')
    {
        g_expr++;
        n *= factor();
    }
    return (n);
}

int expr(void)
{
    int n;

    n = term();
    while (*g_expr == '+')
    {
        g_expr++;
        n += term();
    }
    return (n);
}

int main(int argc, char **argv)
{
    int result;

    if (argc != 2)
        return (1);
    g_expr = argv[1];
    result = expr();
    if (*g_expr != '\0')
        error();
    printf("%d\n", result);
}
