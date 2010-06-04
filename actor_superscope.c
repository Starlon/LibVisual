/* Libvisual-AVS - Advanced visual studio for libvisual
 * 
 * Copyright (C) 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* FIXME TODO:
 *
 * config UI.
 */
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "jsscript.h"
#include "jscntxt.h"
#include "jsapi.h"
#include "script_visscript.h"

#include <libvisual/libvisual.h>

double PI = M_PI;

typedef enum scope_runnable ScopeRunnable;

enum scope_runnable {
    SCOPE_RUNNABLE_INIT,
    SCOPE_RUNNABLE_FRAME,
    SCOPE_RUNNABLE_BEAT,
    SCOPE_RUNNABLE_POINT,
};

static char *scope_fields[] = {
    "INIT", "FRAME", "BEAT", "POINT"
};

JSBool prop_getter(JSContext *cx, JSObject *obj, jsval idval, jsval *vp);
JSBool prop_setter(JSContext *cx, JSObject *obj, jsval idval, jsval *vp);

enum my_tinyid {
    MY_N, MY_B, MY_X, MY_Y, MY_I, MY_V, MY_W, MY_H, MY_RED, MY_GREEN, MY_BLUE, MY_LINESIZE, MY_SKIP, MY_DRAWMODE
};

static JSPropertySpec my_props[] = {
    {"n", MY_N, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"b", MY_B, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"x", MY_X, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"y", MY_Y, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"i", MY_I, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"v", MY_V, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"w", MY_W, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"h", MY_H, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"red", MY_RED, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"green", MY_GREEN, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"blue", MY_BLUE, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"linesize", MY_LINESIZE, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"skip", MY_SKIP, JSPROP_ENUMERATE, prop_getter, prop_setter},
    {"drawmode", MY_DRAWMODE, JSPROP_ENUMERATE, prop_getter, prop_setter},
};

typedef struct {
    /*AvsRunnableContext      *ctx;
    AvsRunnableVariableManager  *vm;
    AvsRunnable         *runnable[4];
    AvsGlobalProxy      *proxy;
    */

    JSContext *ctx;
    JSObject *global;
    JSScript *runnable[4];

    double n, b, x, y, i, v, w, h, red, green, blue, linesize, skip, drawmode; 

    char            *point;
    char            *frame;
    char            *beat;
    char            *init;
    int          channel_source;
    int          draw_type;
    int             color_pos;
    VisPalette       pal;

    int             needs_init;

    VisAudio    *audio;
    PrivateDataOut *data;

    //AVSGfxColorCycler   *cycler;
} SuperScopePrivate;

int lv_superscope_init (VisPluginData *plugin);
int lv_superscope_cleanup (VisPluginData *plugin);
int lv_superscope_requisition (VisPluginData *plugin, int *width, int *height);
int lv_superscope_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
int lv_superscope_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *lv_superscope_palette (VisPluginData *plugin);
int lv_superscope_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

VISUAL_PLUGIN_API_VERSION_VALIDATOR

const VisPluginInfo *get_plugin_info (int *count)
{
    static const VisActorPlugin actor[] = {{
        .requisition = lv_superscope_requisition,
        .palette = lv_superscope_palette,
        .render = lv_superscope_render,
        .vidoptions.depth =
            VISUAL_VIDEO_DEPTH_8BIT |
            VISUAL_VIDEO_DEPTH_32BIT

    }};

    static const VisPluginInfo info[] = {{
        .type = VISUAL_PLUGIN_TYPE_ACTOR, //".[avs]",

        .plugname = "avs_superscope",
        .name = "Libvisual AVS Render: superscope element",
        .author = "Dennis Smit <ds@nerds-incorporated.org>",
        .version = "0.1",
        .about = "The Libvisual AVS Render: superscope element",
        .help = "This is the superscope element for the libvisual AVS system",

        .init = lv_superscope_init,
        .cleanup = lv_superscope_cleanup,
        .events = lv_superscope_events,

        .plugin = VISUAL_OBJECT (&actor[0])
    }};

    *count = sizeof (info) / sizeof (*info);

    return info;
}

JSBool prop_getter(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    SuperScopePrivate *priv = cx->data;

    if (JSVAL_IS_INT(idval)) {
        switch(JSVAL_TO_INT(idval)) {
            case MY_N: 
                *vp = priv->n;
                break;
            case MY_B:
                *vp = priv->b;
                break;
            case MY_X:
                *vp = priv->x;
                break;
            case MY_Y:
                *vp = priv->y;
                break;
            case MY_I:
                *vp = priv->i;
                break;
            case MY_V:
                *vp = priv->v;
                break;
            case MY_W:
                *vp = priv->w;
                break;
            case MY_H:
                *vp = priv->h;
                break;
            case MY_RED:
                *vp = priv->red;
                break;
            case MY_GREEN:
                *vp = priv->green;
                break;
            case MY_BLUE:
                *vp = priv->blue;
                break;
            case MY_LINESIZE:
                *vp = priv->linesize;
                break;
            case MY_SKIP:
                *vp = priv->skip;
                break;
            case MY_DRAWMODE:
                *vp = priv->drawmode;
                break;
            default:
                break;
                       
        }
    } 
    return JS_TRUE;
}

JSBool prop_setter(JSContext *cx, JSObject *obj, jsval idval, jsval *vp)
{
    SuperScopePrivate *priv = cx->data;

    if (JSVAL_IS_INT(idval)) {
        switch(JSVAL_TO_INT(idval)) {
            case MY_N: 
                priv->n = *vp;
                break;
            case MY_B:
                priv->b = *vp;
                break;
            case MY_X:
                priv->x = *vp;
                break;
            case MY_Y:
                priv->y = *vp;
                break;
            case MY_I:
                priv->i = *vp;
                break;
            case MY_V:
                priv->v = *vp;
                break;
            case MY_W:
                priv->w = *vp;
                break;
            case MY_H:
                priv->h = *vp;
                break;
            case MY_RED:
                priv->red = *vp;
                break;
            case MY_GREEN:
                priv->green = *vp;
                break;
            case MY_BLUE:
                priv->blue = *vp;
                break;
            case MY_LINESIZE:
                priv->linesize = *vp;
                break;
            case MY_SKIP:
                priv->skip = *vp;
                break;
            case MY_DRAWMODE:
                priv->drawmode = *vp;
                break;
            default:
                break;
                       
        }
    } 

    return JS_TRUE;
}

int scope_load_runnable(SuperScopePrivate *priv, ScopeRunnable runnable, char *buf)
{
    JSScript *script = JS_CompileScript(priv->ctx, priv->global, buf, strlen(buf), scope_fields[runnable], 0); 

    //JSObject *scrobj = JS_NewScriptObject(priv->ctx, script);
    //JS_AddNamedRoot(priv->ctx, &scrobj, "scrobj");

    priv->runnable[runnable] = script;

    return 0;
}

int scope_run(SuperScopePrivate *priv, ScopeRunnable runnable)
{
    jsval result;
    JS_ExecuteScript(priv->ctx, priv->global, priv->runnable[runnable], &result);

    return 0;
}


int lv_superscope_init (VisPluginData *plugin)
{
    VisScript *script;
    int i;
    VisParamContainer *paramcontainer = visual_plugin_get_params (plugin);
    SuperScopePrivate *priv = visual_mem_new0(SuperScopePrivate, 1);

    static VisParamEntry params[] = {
        VISUAL_PARAM_LIST_ENTRY_STRING ("point", "d=i+v*0.2; r=t+i*$PI*4; x = cos(r)*d; y = sin(r) * d;"),
        VISUAL_PARAM_LIST_ENTRY_STRING ("frame", "t=t-0.01;"),
        VISUAL_PARAM_LIST_ENTRY_STRING ("beat", ""),
        VISUAL_PARAM_LIST_ENTRY_STRING ("init", "n=800;"),
        VISUAL_PARAM_LIST_ENTRY_INTEGER ("channel source", 0),
        VISUAL_PARAM_LIST_ENTRY ("palette"),
        VISUAL_PARAM_LIST_ENTRY_INTEGER ("draw type", 0),
        VISUAL_PARAM_LIST_END
    };

    visual_param_container_add_many (paramcontainer, params);
    
    script = visual_script_get_script(0);

    visual_log_return_val_if_fail(script != NULL, -VISUAL_ERROR_GENERAL);
    
    priv->data = VISUAL_SCRIPT_PLUGIN(script->plugin->info->plugin)->get_data(script->plugin);

    visual_log_return_val_if_fail((priv->data && priv->data->ctx != NULL), -VISUAL_ERROR_GENERAL);

    priv = visual_mem_new0 (SuperScopePrivate, 1);

    priv->data->ctx->data = (void *)priv;

    JS_DefineProperties(priv->data->ctx, priv->data->global, my_props);

    //priv->proxy = visual_object_get_private(VISUAL_OBJECT(plugin));
    //visual_object_ref(VISUAL_OBJECT(priv->proxy));
    visual_object_set_private (VISUAL_OBJECT (plugin), priv);

    visual_palette_allocate_colors (&priv->pal, 1);

    for (i = 0; i < priv->pal.ncolors; i++) {
        priv->pal.colors[i].r = 0xff;
        priv->pal.colors[i].g = 0xff;
        priv->pal.colors[i].b = 0xff;
    }

    visual_param_entry_set_palette (visual_param_container_get (paramcontainer, "palette"), &priv->pal);

    visual_palette_free_colors (&priv->pal);


    /* Init super scope */
//    priv->ctx = avs_runnable_context_new();
//    priv->vm = avs_runnable_variable_manager_new();

    /* Bind variables to context */
/*    avs_runnable_variable_bind(priv->vm, "n", &priv->n);
    avs_runnable_variable_bind(priv->vm, "b", &priv->b);
    avs_runnable_variable_bind(priv->vm, "x", &priv->x);
    avs_runnable_variable_bind(priv->vm, "y", &priv->y);
    avs_runnable_variable_bind(priv->vm, "i", &priv->i);
    avs_runnable_variable_bind(priv->vm, "v", &priv->v);
    avs_runnable_variable_bind(priv->vm, "w", &priv->w);
    avs_runnable_variable_bind(priv->vm, "h", &priv->h);
    avs_runnable_variable_bind(priv->vm, "$PI", &PI);
    avs_runnable_variable_bind(priv->vm, "red", &priv->red);
    avs_runnable_variable_bind(priv->vm, "green", &priv->green);
    avs_runnable_variable_bind(priv->vm, "blue", &priv->blue);
    avs_runnable_variable_bind(priv->vm, "linesize", &priv->linesize);
    avs_runnable_variable_bind(priv->vm, "skip", &priv->skip);
    avs_runnable_variable_bind(priv->vm, "drawmode", &priv->drawmode);
*/
    return 0;
}

int lv_superscope_cleanup (VisPluginData *plugin)
{
    SuperScopePrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

    if(priv->point != NULL)
        visual_mem_free(priv->point);

    if(priv->frame != NULL)
        visual_mem_free(priv->frame);

    if(priv->beat != NULL)
        visual_mem_free(priv->beat);

    if(priv->init != NULL)
        visual_mem_free(priv->init);

/*    if(priv->proxy != NULL)
        visual_object_unref(VISUAL_OBJECT(priv->proxy));
*/
    visual_mem_free (priv);

    return 0;
}

int lv_superscope_requisition (VisPluginData *plugin, int *width, int *height)
{
    return 0;
}

int lv_superscope_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
    visual_video_set_dimension (video, width, height);

    return 0;
}

int lv_superscope_events (VisPluginData *plugin, VisEventQueue *events)
{
    SuperScopePrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
    VisParamEntry *param;
    VisEvent ev;

    while (visual_event_queue_poll (events, &ev)) {
        switch (ev.type) {
            case VISUAL_EVENT_RESIZE:
                lv_superscope_dimension (plugin, ev.event.resize.video,
                        ev.event.resize.width, ev.event.resize.height);
                break;

            case VISUAL_EVENT_PARAM:
                param = ev.event.param.param;

                if (visual_param_entry_is (param, "point")) {

                    if(priv->point != NULL)
                        visual_mem_free(priv->point);

                    priv->point = strdup(visual_param_entry_get_string (param));
                    scope_load_runnable(priv, SCOPE_RUNNABLE_POINT, priv->point);

                } else if (visual_param_entry_is (param, "frame")) {

                    if(priv->frame != NULL)
                        visual_mem_free(priv->frame);

                    priv->frame = strdup(visual_param_entry_get_string (param));
                    scope_load_runnable(priv, SCOPE_RUNNABLE_FRAME, priv->frame);

                } else if (visual_param_entry_is (param, "beat")) {

                    if(priv->beat != NULL)
                        visual_mem_free(priv->beat);

                    priv->beat = strdup(visual_param_entry_get_string (param));
                    scope_load_runnable(priv, SCOPE_RUNNABLE_BEAT, priv->beat);

                } else if (visual_param_entry_is (param, "init")) {

                    if(priv->init != NULL)
                        visual_mem_free(priv->init);

                    priv->init = strdup(visual_param_entry_get_string (param));
                    scope_load_runnable(priv, SCOPE_RUNNABLE_INIT, priv->init);
                    priv->needs_init = TRUE;

                } else if (visual_param_entry_is (param, "channel source"))

                    priv->channel_source = visual_param_entry_get_integer (param);

                else if (visual_param_entry_is (param, "draw type"))

                    priv->draw_type = visual_param_entry_get_integer (param);

                else if (visual_param_entry_is (param, "palette")) {

                    VisPalette *pal;

                    pal = visual_param_entry_get_palette (param);

                    visual_palette_free_colors (&priv->pal);
                    visual_palette_allocate_colors (&priv->pal, pal->ncolors);
                    visual_palette_copy (&priv->pal, pal);

                /*
                    if (priv->cycler != NULL)
                        visual_object_unref (VISUAL_OBJECT (priv->cycler));

                    priv->cycler = avs_gfx_color_cycler_new (&priv->pal);
                    avs_gfx_color_cycler_set_mode (priv->cycler, AVS_GFX_COLOR_CYCLER_TYPE_TIME);
                    avs_gfx_color_cycler_set_time (priv->cycler, avs_config_standard_color_cycler_time ());
                */
                }

                break;

            default:
                break;
        }
    }

    return 0;
}
/*
static int makeint(AvsNumber t)
{
    if (t <= 0.0)
        return 0;
    if (t >= 1.0)
        return 255;
    return (int)(t*255.0);
}
*/
VisPalette *lv_superscope_palette (VisPluginData *plugin)
{
    //SuperScopePrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

    return NULL;
}

static __inline int makeint(double t)
{
  if (t <= 0.0) return 0;
  if (t >= 1.0) return 255;
  return (int)(t*255.0);
}

int lv_superscope_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
    SuperScopePrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
    //AvsGlobalProxy *proxy = priv->proxy;
    uint32_t *buf = visual_video_get_pixels (video);
    int isBeat;

    VisBuffer pcm;
    float pcmbuf[1024];

    printf("lv_superscope_render %p\n", video);

    visual_buffer_set_data_pair (&pcm, pcmbuf, sizeof (pcmbuf));

    visual_audio_get_sample_mixed (audio, &pcm, TRUE, 2,
            VISUAL_AUDIO_CHANNEL_LEFT,
            VISUAL_AUDIO_CHANNEL_RIGHT,
            1.0,
            1.0);


    isBeat = visual_audio_is_beat_with_data(audio, VISUAL_BEAT_ALGORITHM_ADV, pcmbuf, 1024);

    /* Provide audio for AvsRunnable */
    priv->audio = audio;

    if(priv->needs_init) {
        priv->needs_init = FALSE;
        scope_run(priv, SCOPE_RUNNABLE_INIT);
    }

//  visual_video_fill_color(video, visual_color_black()); 
    buf = visual_video_get_pixels (video);

    int a, l, lx = 0, ly = 0, x = 0, y = 0;
    int32_t current_color;
    //int ws=(priv->channel_source&4)?1:0;
    //int xorv=(ws*128)^128;
    //uint16_t fa_data[576];

    priv->color_pos++;

    if(priv->color_pos >= priv->pal.ncolors * 64) priv->color_pos = 0;

    {
        int p = priv->color_pos/64;
        int r = priv->color_pos&63;
        int c1, c2;
        int r1, r2, r3;
        c1 = visual_color_to_uint32(&priv->pal.colors[p]);
        if(p+1 < priv->pal.ncolors)
            c2=visual_color_to_uint32(&priv->pal.colors[p+1]);
        else c2 = visual_color_to_uint32(&priv->pal.colors[0]);

        r1 = (((c1&255)*(63-r))+((c2&255)*4))/64;
        r2 = ((((c1>>8)&255)*(63-r))+(((c2>>8)&255)*4))/64;
        r3 = ((((c1>>16)&255)*(63-r))+(((c2>>16)&255)*r))/64;

        current_color = r1|(r2<<8)|(r3<<16)|(255<<24);
        printf("r1 %d, r2 %d, r3 %d, current_color %x\n", r1, r2, r3, current_color);
    }

    priv->h = video->height;
    priv->w = video->width;
    priv->b = isBeat?1.0:0.0;
    priv->blue = (current_color&0xff)/255.0;
    priv->green = ((current_color>>8)&0xff)/255.0;
    priv->red = ((current_color>>16)&0xff)/255.0;
    priv->skip = 0.0;
    priv->linesize = 1; //(double) ((priv->proxy->line_blend_mode&0xff0000)>>16);
    priv->drawmode = priv->draw_type ? 1.0 : 0.0;

    scope_run(priv, SCOPE_RUNNABLE_FRAME);

    if (isBeat)
        scope_run(priv, SCOPE_RUNNABLE_BEAT);

    //int candraw=0;
    l = priv->n;
    if (l > 128*1024)
        l = 128*1024;


    for (a=0; a < l; a++) 
    {
        //double r=(a*576.0)/l;
        //double s1=r-(int)r;
        //double yr=(fa_data[(int)r]^xorv)*(1.0f-s1)+(fa_data[(int)r+1]^xorv)*(s1);
        //priv->v = yr/128.0 - 1.0;
        priv->v = pcmbuf[a * 288 / l];
        priv->i = a/(l-1);
        priv->skip = 0.0;
        scope_run(priv, SCOPE_RUNNABLE_POINT);

        x = (int)((priv->x + 1.0) * video->width * 0.5);
        y = (int)((priv->y + 1.0) * video->height * 0.5);

        if (priv->skip >= 0.00001)
            continue;

        int this_color = makeint(priv->blue) | (makeint(priv->green) << 8) | (makeint(priv->red) << 16);

        if (priv->drawmode < 0.00001) {
                //if (y >= 0 && y < video->height && x >= 0 && x < video->width)
                //BLEND_LINE(proxy, buf+x+y*video->width, this_color);
        } else {
            if (a > 0) {
                if (y >= 0 && y < video->height && x >= 0 && x < video->width &&
                    ly >= 0 && ly < video->height && lx >= 0 && lx < video->width) {
                        VisColor color;
                        //this_color = 0xffffff;
                        visual_color_from_uint32(&color, this_color);
                        avs_gfx_line_ints(video, lx, ly, x, y, &color);
                }
            }
        }
        lx = x;
        ly = y;
    }

    return 0;
}

