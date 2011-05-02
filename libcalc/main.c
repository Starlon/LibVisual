#include <stdio.h>
#include <glib.h>
#include "calc.h"

int main(int argc, char **argv)
{
    double *n, *t;
    int i;

    symbol_dict_t *environment = dict_new();

    n = dict_variable(environment, "test");
    *n = 50;

    expression_t *expr = expr_compile_string("test=test+100; t = t + 0.5; test=test*t;", environment);;

    t = dict_variable(environment, "t");
    expr_execute(expr, environment);
    expr_execute(expr, environment);
    expr_execute(expr, environment);


    n = dict_variable(environment, "test");
    i = dict_lookup(environment, "test");

    printf(".value = %f, *n = %f, i = %d, t = %f\n", environment->variables[i].value, *n, i, *t);
    
    return 0;
}
