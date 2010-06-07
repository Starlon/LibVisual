
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "evaluator.h"

double PI = 3.14159;
double E = 2.71828;
double PHI = 1.618033;

static void function_log(RESULT * result, RESULT * arg1);
static void function_sin(RESULT * result, RESULT * arg1);
static void function_cos(RESULT * result, RESULT * arg1);
static void function_tan(RESULT * result, RESULT * arg1);
static void function_asin(RESULT * result, RESULT * arg1);
static void function_acos(RESULT * result, RESULT * arg1);
static void function_atan(RESULT * result, RESULT * arg1);
static void function_if(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3);
static void function_div(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_rand(RESULT * result, RESULT * arg1, RESULT * arg2);

void init_evaluator()
{
        RESULT *result = malloc(sizeof(RESULT));

        AddFunction("log", 1, function_log);
        AddFunction("sin", 1, function_sin);
        AddFunction("cos", 1, function_cos);
        AddFunction("tan", 1, function_tan);
        AddFunction("asin", 1, function_asin);
        AddFunction("acos", 1, function_acos);
        AddFunction("atan", 1, function_atan);
        AddFunction("if", 1, function_if);
        AddFunction("div", 1, function_div);
        AddFunction("rand", 1, function_rand);

        SetResult(&result, R_NUMBER, &PI);
        SetVariable ("PI", result);

        SetResult(&result, R_NUMBER, &E);
        SetVariable ("E", result);

        SetResult(&result, R_NUMBER, &PHI);
        SetVariable ("PHI", result);

        free(result);
}

static void function_log(RESULT * result, RESULT * arg1)
{
        double val = log(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_sin(RESULT * result, RESULT * arg1)
{
        double val = sin(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_cos(RESULT * result, RESULT * arg1)
{
        double val = cos(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_tan(RESULT * result, RESULT * arg1)
{
        double val = tan(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_asin(RESULT * result, RESULT * arg1)
{
        double val = asin(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_acos(RESULT * result, RESULT * arg1)
{
        double val = acos(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_atan(RESULT * result, RESULT * arg1)
{
        double val = atan(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_if(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3)
{
        double a = R2N(arg1);
        double b = R2N(arg2);
        double c = R2N(arg3);
        double val = (c != 0.0) ? a : b;

        SetResult(&result, R_NUMBER, &val);
}

static void function_div(RESULT * result, RESULT * arg1, RESULT * arg2)
{
        double a = R2N(arg1);
        double b = R2N(arg2);
        double val = (a == 0) ? 0 : (b / a);
        SetResult(&result, R_NUMBER, &val);
}

static void function_rand(RESULT * result, RESULT * arg1, RESULT * arg2)
{
        int a, b, seed, val;
        a = R2N(arg1);
        b = R2N(arg1);

        seed = time(NULL);

        srand(seed);

        val = (rand() % (b - a)) + a;

        SetResult(&result, R_NUMBER, &val);
}

