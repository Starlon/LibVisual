#include <stdio.h>
#include <glib.h>
#include "calc.h"

int main(int argc, char **argv)
{
    double *n;
    int i;

    symbol_dict_t *environment = dict_new();

    n = dict_variable(environment, "test");
    *n = 50;

    expression_t *expr = expr_compile_string("test=test+100; ", environment);;

    expr_execute(expr, environment);

    n = dict_variable(environment, "test");
    i = dict_lookup(environment, "test");

    printf(".value = %f, *n = %f, i = %d\n", environment->variables[i].value, *n, i);
    
    return 0;
}
