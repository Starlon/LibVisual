#include <stdio.h>
#include <glib.h>
#include "calc.h"

int main(int argc, char **argv)
{
    double *n, *t;
    int i = 0;

    symbol_dict_t *environment = dict_new();

    n = dict_variable(environment, "test");
    *n = 50;

    expression_t *expr = expr_compile_string("t = if(t, 0, 1);", environment);;

    t = dict_variable(environment, "t");
    printf("*n = %f, i = %d, t = %f\n", *n, i, *t);
    expr_execute(expr, environment);
    printf("*n = %f, i = %d, t = %f\n", *n, i, *t);
    expr_execute(expr, environment);
    printf("*n = %f, i = %d, t = %f\n", *n, i, *t);
    expr_execute(expr, environment);
    printf("*n = %f, i = %d, t = %f\n", *n, i, *t);


    
    return 0;
}
