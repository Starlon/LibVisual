#include "evaluator.h"
#include "visscript.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    void *tree;
    double d = 1;
    RESULT *result = malloc(sizeof(RESULT));

    SetResult(&result, R_NUMBER, &d);

    init_evaluator();

    SetVariable("n", result);
    SetVariable("b", result);
    SetVariable("x", result);
    SetVariable("y", result);
    SetVariable("i", result);
    SetVariable("v", result);
    SetVariable("w", result);
    SetVariable("h", result);
    SetVariable("t", result);
    SetVariable("d", result);
    SetVariable("red", result);
    SetVariable("green", result);
    SetVariable("blue", result);
    SetVariable("linesize", result);
    SetVariable("skip", result);
    SetVariable("drawmode", result);

    Compile("d=i+v*0.2; r=t+i*PI*4; x=cos(r)*d; y=sin(r)*d", &tree);

    Eval(tree, result);

    RESULT *var = GetVariable("d");

    var = GetVariable("d");
    printf("d = %f\n", var->number);
    var = GetVariable("r");
    printf("r = %f\n", var->number);
    var = GetVariable("x");
    printf("x = %f\n", var->number);
    var = GetVariable("y");
    printf("y = %f\n", var->number);

    free(result);

    return 0;
}
