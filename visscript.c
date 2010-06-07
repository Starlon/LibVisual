
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "evaluator.h"

double PI = 3.14159;
double E = 2.71828;
double PHI = 1.618033;

#define isnonzero(x) (fabs(x) > 0.00001)
#define sqr(x) ((x)*(x))

static void function_log(RESULT * result, RESULT * arg1);
static void function_sin(RESULT * result, RESULT * arg1);
static void function_cos(RESULT * result, RESULT * arg1);
static void function_tan(RESULT * result, RESULT * arg1);
static void function_asin(RESULT * result, RESULT * arg1);
static void function_acos(RESULT * result, RESULT * arg1);
static void function_atan(RESULT * result, RESULT * arg1);
static void function_if(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3);
static void function_div(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_rand(RESULT * result, RESULT * arg1);
static void function_equal(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_abs(RESULT * result, RESULT * arg1);

static void function_atan2(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_sqr(RESULT * result, RESULT * arg1);
static void function_sqrt(RESULT * result, RESULT * arg1);
static void function_invsqrt(RESULT * result, RESULT * arg1);
static void function_pow(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_exp(RESULT * result, RESULT * arg1);
static void function_floor(RESULT * result, RESULT * arg1);
static void function_ceil(RESULT * result, RESULT * arg1);
static void function_sign(RESULT * result, RESULT * arg1);
static void function_min(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_max(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_sigmoid(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_band(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_bor(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_bnot(RESULT * result, RESULT * arg1);
static void function_above(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_below(RESULT * result, RESULT * arg1, RESULT * arg2);

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
        AddFunction("if", 3, function_if);
        AddFunction("div", 2, function_div);
        AddFunction("rand", 1, function_rand);
        AddFunction("equal", 2, function_equal);
        AddFunction("abs", 1, function_abs);
        AddFunction("atan2", 2, function_atan2);
        AddFunction("sqr", 1, function_sqr);
        AddFunction("sqrt", 1, function_sqrt);
        AddFunction("invsqrt", 1, function_invsqrt);
        AddFunction("pow", 2, function_pow);
        AddFunction("exp", 1, function_exp);
        AddFunction("floor", 1, function_floor);
        AddFunction("ceil", 1, function_ceil);
        AddFunction("sign", 1, function_sign);
        AddFunction("min", 2, function_min);
        AddFunction("max", 2, function_max);
        AddFunction("sigmoid", 2, function_sigmoid);
        AddFunction("band", 2, function_band);
        AddFunction("bor", 2, function_bor);
        AddFunction("bnot", 1, function_bnot);
        AddFunction("above", 2, function_above);
        AddFunction("below", 2, function_below);

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

static void function_rand(RESULT * result, RESULT * arg1)
{
        int a, seed, val;
        a = R2N(arg1);

        seed = time(NULL);

        srand(seed);

        if( a == 0) 
            val = rand();
        else
            val = rand() % a;

        SetResult(&result, R_NUMBER, &val);
}

static void function_equal(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val = a == b;

    SetResult(&result, R_NUMBER, &val);
}

static void function_abs(RESULT * result, RESULT * arg1)
{
    double val = abs(R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_atan2(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val;
   
    if( b != 0)
       val = atan(a/b);
    else
        val = 0;

    SetResult(&result, R_NUMBER, &val);
}

static void function_sqr(RESULT * result, RESULT * arg1)
{
    double val = sqr(R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_sqrt(RESULT * result, RESULT * arg1)
{
    double val = sqrt(R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_invsqrt(RESULT * result, RESULT * arg1)
{
    double a = sqrt(R2N(arg1));
    double val = (a == 0.0) ? 0 : 1/sqrt(R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_pow(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double val = pow(R2N(arg1), R2N(arg2));

    SetResult(&result, R_NUMBER, &val);
}

static void function_exp(RESULT * result, RESULT * arg1)
{
    double val = pow(E, R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_floor(RESULT * result, RESULT * arg1)
{
    double val = floor(R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_ceil(RESULT * result, RESULT * arg1)
{
    double val = ceil(R2N(arg1));

    SetResult(&result, R_NUMBER, &val);
}

static void function_sign(RESULT * result, RESULT * arg1)
{
    double a = R2N(arg1);
    double val;
    
    if( a > 0 )
        val = 1;
    else if( a < 0)
        val = -1;
    else
        val = 0;

    SetResult(&result, R_NUMBER, &val);
}

static void function_min(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val;

    if( a < b )
        val = a;
    else
        val = b;

    SetResult(&result, R_NUMBER, &val);
}

static void function_max(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val;

    if( a > b )
        val = a;
    else
        val = b;

    SetResult(&result, R_NUMBER, &val);
}

static void function_sigmoid(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double t = 1+exp(-a * b);
    double val = isnonzero(t) ? 1.0/t : 0;

    SetResult(&result, R_NUMBER, &val);
}

static void function_band(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val = isnonzero(a) && isnonzero(b) ? 1 : 0;

    SetResult(&result, R_NUMBER, &val);
}

static void function_bor(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val = isnonzero(a) || isnonzero(b) ? 1 : 0;

    SetResult(&result, R_NUMBER, &val);
}

static void function_bnot(RESULT * result, RESULT * arg1)
{
    double val = ~(int)R2N(arg1);

    SetResult(&result, R_NUMBER, &val);
}

static void function_above(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val;

    val = a > b ? 1 : 0;

    SetResult(&result, R_NUMBER, &val);
}

static void function_below(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    double a = R2N(arg1);
    double b = R2N(arg2);
    double val;

    val = a < b ? 1 : 0;

    SetResult(&result, R_NUMBER, &val);
}

