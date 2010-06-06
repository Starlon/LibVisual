
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <v8.h>
#include "visscript_v8.h"

using namespace v8;

static Handle<Value> function_log(const Arguments &args);
static Handle<Value> function_sin(const Arguments &args);
static Handle<Value> function_cos(const Arguments &args);
static Handle<Value> function_tan(const Arguments &args);
static Handle<Value> function_asin(const Arguments &args);
static Handle<Value> function_acos(const Arguments &args);
static Handle<Value> function_atan(const Arguments &args);
static Handle<Value> function_if(const Arguments &args);
static Handle<Value> function_div(const Arguments &args);
static Handle<Value> function_rand(const Arguments &args);

v8Script::v8Script()
{
        HandleScope handle_scope;

        global_ = Persistent<ObjectTemplate>::New(ObjectTemplate::New());
        context_ = Persistent<Context>::New(Context::New(NULL, global_));
        Context::Scope context_scope(context_);

        global_->Set(String::New("log"), FunctionTemplate::New(function_log));
        global_->Set(String::New("sin"), FunctionTemplate::New(function_sin));
        global_->Set(String::New("cos"), FunctionTemplate::New(function_cos));
        global_->Set(String::New("tan"), FunctionTemplate::New(function_tan));
        global_->Set(String::New("asin"), FunctionTemplate::New(function_asin));
        global_->Set(String::New("acos"), FunctionTemplate::New(function_acos));
        global_->Set(String::New("atan"), FunctionTemplate::New(function_atan));
        global_->Set(String::New("if"), FunctionTemplate::New(function_if));
        global_->Set(String::New("div"), FunctionTemplate::New(function_div));
        global_->Set(String::New("rand"), FunctionTemplate::New(function_rand));
}

v8Script::~v8Script() 
{
    context_.Dispose();
}

static Handle<Value> function_log(const Arguments &args)
{
	
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

        printf("function_log %f\n", val->ToNumber()->Value());

	return val;
}

static Handle<Value> function_sin(const Arguments &args)
{
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_cos(const Arguments &args)
{
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();


        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_tan(const Arguments &args)
{
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_asin(const Arguments &args)
{
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_acos(const Arguments &args)
{
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_atan(const Arguments &args)
{
        HandleScope handle_scope;

	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_if(const Arguments &args)
{
        HandleScope handle_scope;

	double a, b, c;

	if (args.Length() != 3) 
		return v8::Undefined();

        a = args[0]->NumberValue();
        b = args[1]->NumberValue();
        c = args[2]->NumberValue();
        Handle<Value> val = Number::New((c != 0.0) ? a : b);

	return val;
}

static Handle<Value> function_div(const Arguments &args)
{
        HandleScope handle_scope; 
	double a, b;

	if (args.Length() != 2) 
		return v8::Undefined();

        a = args[0]->NumberValue();
        b = args[1]->NumberValue();
        Handle<Value> val = Number::New((a == 0) ? 0 : (b / a));

	return val;
}

static Handle<Value> function_rand(const Arguments &args)
{
        HandleScope handle_scope; 
	int a, b, seed;
	
        if  (args.Length() != 2)
            return v8::Undefined();


        a = args[0]->NumberValue();
        b = args[1]->NumberValue();

	seed = time(NULL);

	srand(seed);
        Handle<Value> val = Number::New((rand()%(b-a))+a);

	return val;
}
