#include <config.h>

#include "lv_script.h"

#include "lvconfig.h"

VisList *__lv_script = NULL;
extern VisList *__lv_plugins_script;

VisPluginData *visual_script_get_plugin (VisScript *script)
{
    return script->plugin;
}

VisList *visual_script_get_list (void) 
{
	return __lv_plugins_script;
}


VisScript *visual_script_new (const char *scriptname) 
{
	VisScript *script;
	script = visual_mem_new0 (VisScript, 1);
	
	visual_script_init (script, scriptname);

	visual_object_set_allocated (VISUAL_OBJECT (script), TRUE);
	visual_object_ref (VISUAL_OBJECT (script));

	return script;
}

int visual_script_init (VisScript *script, const char *scriptname)
{
	VisPluginRef *ref;

        if(__lv_script == NULL) {
            __lv_script = visual_list_new(visual_object_collection_destroyer);
        }

	visual_log_return_val_if_fail (script != NULL, -VISUAL_ERROR_SCRIPT_NULL);

	ref = visual_plugin_find (__lv_plugins_script, scriptname);

	script->plugin = visual_plugin_load (ref);

        visual_list_add(__lv_script, script);

        visual_script_realize(script);

	return VISUAL_OK;
}

int visual_script_realize (VisScript *script)
{
	visual_log_return_val_if_fail (script != NULL, -VISUAL_ERROR_MORPH_NULL);
	visual_log_return_val_if_fail (script->plugin != NULL, -VISUAL_ERROR_PLUGIN_NULL);

	return visual_plugin_realize (script->plugin);
}

VisScript *visual_script_get_script(int num)
{
	visual_log_return_val_if_fail(num < visual_list_count(__lv_script), NULL);
        VisScript *script = visual_list_get(__lv_script, num);

        return script;

}

