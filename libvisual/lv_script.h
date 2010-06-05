#ifndef _LV_SCRIPT_H
#define _LV_SCRIPT_H


#include <libvisual/lv_plugin.h>

#define VISUAL_PLUGIN_TYPE_SCRIPT	"Libvisual:core:script"

#define VISUAL_SCRIPT(obj)  (VISUAL_CHECK_CAST ((obj), VisScript))
#define VISUAL_SCRIPT_PLUGIN(obj)   (VISUAL_CHECK_CAST((obj), VisScriptPlugin))

typedef struct _VisScript VisScript;
typedef struct _VisScriptPlugin VisScriptPlugin;

typedef void *(*VisPluginScriptGetData)(VisPluginData *plugin);

struct _VisScript {
	VisObject object;	/**< The VisObject data. */
	VisPluginData *plugin;	/**< Pointer to the plugin itself. */
};

struct _VisScriptPlugin {
	VisObject object;	/**<The VisObject data. */
	VisPluginScriptGetData get_data; /**< Script data */
};

VisPluginData *visual_script_get_plugin (VisScript *script);

VisList *visual_script_get_list (void);

VisScript *visual_script_new (const char *scriptname);
int visual_script_init(VisScript *script, const char *scriptname);

int visual_script_realize (VisScript *morph);

VisScript *visual_script_get_script(int num);

#endif
