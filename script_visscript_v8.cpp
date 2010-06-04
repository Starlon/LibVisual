
#include <libvisual/libvisual.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <v8.h>
#include "script_visscript_v8.h"

VISUAL_PLUGIN_API_VERSION_VALIDATOR

using namespace v8;

typedef struct {
	HandleScope handle_scope;
} VisscriptPrivate;

int script_visscript_init (VisPluginData *plugin);
int script_visscript_cleanup (VisPluginData *plugin);

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

static int data_dtor(VisObject *object)
{
    PrivateDataOut *data = PRIVATE_DATA(object);

    return VISUAL_OK;
}

void * get_data(VisPluginData *plugin)
{
	VisscriptPrivate *priv = (VisscriptPrivate *)visual_object_get_private(VISUAL_OBJECT(plugin));
        PrivateDataOut *data = (PrivateDataOut *)visual_mem_new0(PrivateDataOut, 1);
        Handle<ObjectTemplate> global;
        Persistent<Context> *context;

        visual_log_return_val_if_fail(priv != NULL, NULL);

        visual_object_set_dtor (VISUAL_OBJECT(data), data_dtor);
        visual_object_set_allocated (VISUAL_OBJECT(data), TRUE);

        global = ObjectTemplate::New();
        context = &Context::New(NULL, global);
        data->context = (void *)context;
        global->Set(String::New("log"), FunctionTemplate::New(function_log));
        global->Set(String::New("sin"), FunctionTemplate::New(function_sin));
        global->Set(String::New("cos"), FunctionTemplate::New(function_cos));
        global->Set(String::New("tan"), FunctionTemplate::New(function_tan));
        global->Set(String::New("asin"), FunctionTemplate::New(function_asin));
        global->Set(String::New("acos"), FunctionTemplate::New(function_acos));
        global->Set(String::New("atan"), FunctionTemplate::New(function_atan));
        global->Set(String::New("if"), FunctionTemplate::New(function_if));
        global->Set(String::New("div"), FunctionTemplate::New(function_div));
        global->Set(String::New("rand"), FunctionTemplate::New(function_rand));
        //data->global = (void *)global;
	return data;
}

const VisPluginInfo *get_plugin_info(int *count)
{

	static VisScriptPlugin script[] = {{
	}};

        script[0].get_data = get_data;

	static VisPluginInfo info[] = {{
        }};
	info[0].type = VISUAL_PLUGIN_TYPE_SCRIPT;
        info[0].plugname = "visscript-v8";
	info[0].name = "VisScript v8 plugin";
	info[0].author = "Scott Sibley <starlon@sf.net>";
	info[0].version = "0.1";
	info[0].about = "Libvisual javascript plugin";
        info[0].help = "v8 Javascript plugin";
	info[0].license = VISUAL_PLUGIN_LICENSE_GPL;
	info[0].init = script_visscript_init;
	info[0].cleanup = script_visscript_cleanup;
	info[0].plugin = VISUAL_OBJECT (&script[0]);

	*count = sizeof(info) / sizeof(*info);

	return info;
}

int script_visscript_init (VisPluginData *plugin)
{
	VisscriptPrivate *priv = visual_mem_new0(VisscriptPrivate, 1);

	visual_object_set_private(VISUAL_OBJECT(plugin), priv);

	return VISUAL_OK;
}

int script_visscript_cleanup (VisPluginData *plugin)
{
	VisscriptPrivate *priv = (VisscriptPrivate *)visual_object_get_private(VISUAL_OBJECT(plugin));

	if( priv ) 
		visual_mem_free(priv);

	return VISUAL_OK;
}

static Handle<Value> function_log(const Arguments &args)
{
	
	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_sin(const Arguments &args)
{
	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_cos(const Arguments &args)
{
	if (args.Length() != 1) 
		return v8::Undefined();


        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_tan(const Arguments &args)
{
	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_asin(const Arguments &args)
{
	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_acos(const Arguments &args)
{
	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_atan(const Arguments &args)
{
	if (args.Length() != 1) 
		return v8::Undefined();

        Handle<Value> val = Number::New(log(args[0]->NumberValue()));

	return val;
}

static Handle<Value> function_if(const Arguments &args)
{
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
