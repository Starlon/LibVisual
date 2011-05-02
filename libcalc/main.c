#include <stdio.h>
#include <glib.h>
#include "calc.h"

int main(int argc, char **argv)
{
	double *n;

	symbol_dict_t *environment = dict_new();


	n = dict_variable(environment, "test");
        *n = 50;

	expression_t *expr = expr_compile_string("test = 100;", environment);;



	expr_execute(expr, environment);


	printf("n = %f\n", *n);
	
	return 0;
}
