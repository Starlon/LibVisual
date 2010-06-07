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
#include <libvisual/libvisual.h>

#include "evaluator.h"
#include "visscript.h"

#include "avs_common.h"

enum scope_runnable {
    SCOPE_RUNNABLE_INIT,
    SCOPE_RUNNABLE_FRAME,
    SCOPE_RUNNABLE_BEAT,
    SCOPE_RUNNABLE_POINT,
};

typedef enum scope_runnable ScopeRunnable;

typedef struct {
    void *data;
} VoidP;

typedef struct {
    AvsGlobalProxy      *proxy;
    VoidP runnable[4];

    double n, b, x, y, i, v, w, h, t, d, red, green, blue, linesize, skip, drawmode; 

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

    AVSGfxColorCycler   *cycler;
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
    static VisActorPlugin actor[1];

    actor[0].requisition = lv_superscope_requisition;
    actor[0].palette = lv_superscope_palette;
    actor[0].render = lv_superscope_render;
    actor[0].vidoptions.depth = VISUAL_VIDEO_DEPTH_8BIT | VISUAL_VIDEO_DEPTH_32BIT;

    static VisPluginInfo info[1];

    info[0].type = VISUAL_PLUGIN_TYPE_ACTOR, //".[avs]",

    info[0].plugname = "avs_superscope";
    info[0].name = "Libvisual AVS Render: superscope element";
    info[0].author = "Dennis Smit <ds@nerds-incorporated.org>";
    info[0].version = "0.1";
    info[0].about = "The Libvisual AVS Render: superscope element";
    info[0].help = "This is the superscope element for the libvisual AVS system";

    info[0].init = lv_superscope_init;
    info[0].cleanup = lv_superscope_cleanup;
    info[0].events = lv_superscope_events;

    info[0].plugin = VISUAL_OBJECT (&actor[0]);

    *count = sizeof (info) / sizeof (*info);

    return info;
}

int scope_load_runnable(SuperScopePrivate *priv, ScopeRunnable runnable, char *buf)
{
    Compile(buf, &priv->runnable[runnable].data);
    return 0;
}

int scope_run(SuperScopePrivate *priv, ScopeRunnable runnable)
{
    RESULT *result = visual_mem_new0(RESULT, 1);
    Eval(priv->runnable[runnable].data, result);
    visual_mem_free(result);
    return 0;
}


int lv_superscope_init (VisPluginData *plugin)
{
    int i;
    VisParamContainer *paramcontainer = visual_plugin_get_params (plugin);
    SuperScopePrivate *priv = visual_mem_new0(SuperScopePrivate, 1);

    static VisParamEntry params[] = {
        VISUAL_PARAM_LIST_ENTRY_STRING ("point", "d=i+v*0.2; r=t+i*PI*4; x = cos(r)*d; y = sin(r) * d;"),
        VISUAL_PARAM_LIST_ENTRY_STRING ("frame", "t=0;t=t-0.01;"),
        VISUAL_PARAM_LIST_ENTRY_STRING ("beat", ""),
        VISUAL_PARAM_LIST_ENTRY_STRING ("init", "n=800;"),
        VISUAL_PARAM_LIST_ENTRY_INTEGER ("channel source", 0),
        VISUAL_PARAM_LIST_ENTRY ("palette"),
        VISUAL_PARAM_LIST_ENTRY_INTEGER ("draw type", 0),
        VISUAL_PARAM_LIST_END
    };

    visual_param_container_add_many (paramcontainer, params);

    priv->proxy = (AvsGlobalProxy *)visual_object_get_private(VISUAL_OBJECT(plugin));
    visual_object_ref(VISUAL_OBJECT(priv->proxy));

    visual_object_set_private (VISUAL_OBJECT (plugin), priv);

    init_evaluator();

    RESULT *result = visual_mem_new0(RESULT, 1);
    double val = 100;
    SetResult(&result, R_NUMBER, &val);
    SetVariable("n", result);
    val = 0;
    SetResult(&result, R_NUMBER, &val);
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
    visual_mem_free(result);

    visual_palette_allocate_colors (&priv->pal, 1);

    for (i = 0; i < priv->pal.ncolors; i++) {
        priv->pal.colors[i].r = 0xff;
        priv->pal.colors[i].g = 0xff;
        priv->pal.colors[i].b = 0xff;
    }

    visual_param_entry_set_palette (visual_param_container_get (paramcontainer, "palette"), &priv->pal);

    visual_palette_free_colors (&priv->pal);

    return 0;
}

int lv_superscope_cleanup (VisPluginData *plugin)
{
    SuperScopePrivate *priv = (SuperScopePrivate *)visual_object_get_private (VISUAL_OBJECT (plugin));

    if(priv->point != NULL)
        visual_mem_free(priv->point);

    if(priv->frame != NULL)
        visual_mem_free(priv->frame);

    if(priv->beat != NULL)
        visual_mem_free(priv->beat);

    if(priv->init != NULL)
        visual_mem_free(priv->init);

    if(priv->proxy != NULL)
        visual_object_unref(VISUAL_OBJECT(priv->proxy));

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
    SuperScopePrivate *priv = (SuperScopePrivate *)visual_object_get_private (VISUAL_OBJECT (plugin));
    VisParamEntry *param;
    VisEvent ev;

    while (visual_event_queue_poll (events, &ev)) {
        switch (ev.type) {
            case VISUAL_EVENT_RESIZE:
                lv_superscope_dimension (plugin, ev.event.resize.video,
                        ev.event.resize.width, ev.event.resize.height);
                break;

            case VISUAL_EVENT_PARAM:
                param = (VisParamEntry *)ev.event.param.param;

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

void set_vars(SuperScopePrivate *priv)
{   
    VARIABLE *var = FindVariable("n");
    priv->n = R2N(var->value);
    var = FindVariable("b");
    priv->b = R2N(var->value);
    var = FindVariable("x");
    priv->x = R2N(var->value);
    var = FindVariable("y");
    priv->y = R2N(var->value);
    var = FindVariable("i");
    priv->i = R2N(var->value);
    var = FindVariable("v");
    priv->v = R2N(var->value);
    var = FindVariable("w");
    priv->w = R2N(var->value);
    var = FindVariable("h");
    priv->h = R2N(var->value);
    var = FindVariable("t");
    priv->t = R2N(var->value);
    var = FindVariable("d");
    priv->d = R2N(var->value);
    var = FindVariable("red");
    priv->red = R2N(var->value);
    var = FindVariable("green");
    priv->green = R2N(var->value);
    var = FindVariable("blue");
    priv->blue = R2N(var->value);
    var = FindVariable("linesize");
    priv->linesize = R2N(var->value);
    var = FindVariable("skip");
    priv->skip = R2N(var->value);
    var = FindVariable("drawmode");
    priv->drawmode = R2N(var->value);
}

static __inline int makeint(double t)
{
  if (t <= 0.0) return 0;
  if (t >= 1.0) return 255;
  return (int)(t*255.0);
}

int lv_superscope_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
    SuperScopePrivate *priv = (SuperScopePrivate *)visual_object_get_private (VISUAL_OBJECT (plugin));
    AvsGlobalProxy *proxy = priv->proxy;
    uint32_t *buf = (uint32_t *)visual_video_get_pixels (video);
    int isBeat;

    VisBuffer pcm;
    float pcmbuf[576];

    /*
    visual_buffer_set_data_pair (&pcm, pcmbuf, sizeof (pcmbuf));

    visual_audio_get_sample_mixed (audio, &pcm, TRUE, 2,
            VISUAL_AUDIO_CHANNEL_LEFT,
            VISUAL_AUDIO_CHANNEL_RIGHT,
            1.0,
            1.0);
    */

    isBeat = priv->proxy->isBeat;

    if(priv->needs_init) {
        priv->needs_init = FALSE;
        scope_run(priv, SCOPE_RUNNABLE_INIT);
        set_vars(priv);
    }

//  visual_video_fill_color(video, visual_color_black()); 
    buf = (uint32_t *)visual_video_get_pixels (video);

    int a, l, lx = 0, ly = 0, x = 0, y = 0;
    int32_t current_color;
    int ws=(priv->channel_source&4)?1:0;
    int xorv=(ws*128)^128;
    uint16_t fa_data[576];

    
    if((priv->channel_source&3) >= 2)
    {
        for(x = 0; x < 576; x++) {
            //fa_data[x] = proxy->audiodata[ws^1][0][x] * UINT8_MAX + proxy->audiodata[ws^1][1][x] * UINT8_MAX;
            pcmbuf[x] = proxy->audiodata[ws^1][0][x] / 2 + proxy->audiodata[ws^1][1][x] / 2;
        }
    }
    else
    {
        printf(" < 2\n");
        for(x = 0; x < 576; x++) {
            //fa_data[x] = proxy->audiodata[ws^1][priv->channel_source&3][x] * UINT16_MAX;
            pcmbuf[x] = proxy->audiodata[ws^1][priv->channel_source&3][x];
        }
    }

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
    }

    priv->h = video->height;
    priv->w = video->width;
    priv->b = isBeat?1.0:0.0;
    priv->blue = (current_color&0xff)/255.0;
    priv->green = ((current_color>>8)&0xff)/255.0;
    priv->red = ((current_color>>16)&0xff)/255.0;
    priv->skip = 0.0;
    priv->linesize = (double) ((priv->proxy->line_blend_mode&0xff0000)>>16);
    priv->drawmode = priv->draw_type ? 1.0 : 0.0;

    
    SetVariableNumeric("h", priv->h);
    SetVariableNumeric("w", priv->w);
    SetVariableNumeric("b", priv->b);
    SetVariableNumeric("blue", priv->blue);
    SetVariableNumeric("green", priv->green);
    SetVariableNumeric("red", priv->red);
    SetVariableNumeric("skip", priv->skip);
    SetVariableNumeric("linesize", priv->linesize);
    SetVariableNumeric("drawmode", priv->drawmode);

    scope_run(priv, SCOPE_RUNNABLE_FRAME);

    if (isBeat) {
        scope_run(priv, SCOPE_RUNNABLE_BEAT);
    }

    set_vars(priv);

    //int candraw=0;
    l = priv->n;
    if (l > 128*1024)
        l = 128*1024;

    if( l == 0 || l == 1 )
        l = 2;

    for (a=0; a < l; a++) 
    {
        //int x, y;
        //double r=(a*576.0)/l;
        //double s1=r-(int)r;
        //double yr=((int)pcmbuf[(int)r]^xorv)*(1.0f-s1)+((int)pcmbuf[(int)r+1]^xorv)*(s1);
        //priv->v = yr/128.0 - 1.0;
        priv->v = pcmbuf[a * 288 / l];
        priv->i = a/(double)(l-1);
        priv->skip = 0.0;
        SetVariableNumeric("v", priv->v);
        SetVariableNumeric("i", priv->i);
        SetVariableNumeric("skip", priv->skip);
        scope_run(priv, SCOPE_RUNNABLE_POINT);
        set_vars(priv);

        x = (int)((priv->x + 1.0) * video->width * 0.5);
        y = (int)((priv->y + 1.0) * video->height * 0.5);

        if (priv->skip >= 0.00001)
            continue;

        int this_color = makeint(priv->blue) | (makeint(priv->green) << 8) | (makeint(priv->red) << 16);

        if (1) {//priv->drawmode < 0.00001) {
                if (y >= 0 && y < video->height && x >= 0 && x < video->width)
                    BLEND_LINE(proxy, buf+x+y*video->width, this_color);
        } else {
            if (a > 0) {
                if (y >= 0 && y < video->height && x >= 0 && x < video->width &&
                    ly >= 0 && ly < video->height && lx >= 0 && lx < video->width) {
                        VisColor color;
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

