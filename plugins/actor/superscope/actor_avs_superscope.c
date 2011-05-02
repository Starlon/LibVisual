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

#include "calc.h"
//#include "evaluator.h"
//#include "visscript.h"

#include "avs_common.h"
#include "lvavs_pipeline.h"

enum scope_runnable {
    SCOPE_RUNNABLE_INIT,
    SCOPE_RUNNABLE_FRAME,
    SCOPE_RUNNABLE_BEAT,
    SCOPE_RUNNABLE_POINT,
};

typedef enum scope_runnable ScopeRunnable;

/*
typedef struct {
    void *data;
} VoidP;
*/

/*
typedef struct {
   expression_t *expression;
   symbol_dict_t *dict;
} AvsRunnable;
*/

typedef struct {
    LVAVSPipeline *pipeline;
    expression_t *runnable[4];
    symbol_dict_t *environment;

    double *n, *b, *x, *y, *i, *v, *w, *h, *t, *d, *red, *green, *blue, *linesize, *skip, *drawmode; 

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
    priv->runnable[runnable] = expr_compile_string(buf, priv->environment);
    return 0;
}

int scope_run(SuperScopePrivate *priv, ScopeRunnable runnable)
{
    expr_execute(priv->runnable[runnable], priv->environment);

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

    priv->pipeline = (LVAVSPipeline *)visual_object_get_private(VISUAL_OBJECT(plugin));
    visual_object_ref(VISUAL_OBJECT(priv->pipeline));

    visual_object_set_private (VISUAL_OBJECT (plugin), priv);

    priv->environment = dict_new();

    /* initialize variables */
    priv->n = dict_variable(priv->environment, "n");
    priv->b = dict_variable(priv->environment, "b");
    priv->x = dict_variable(priv->environment, "x");
    priv->y = dict_variable(priv->environment, "y");
    priv->i = dict_variable(priv->environment, "i");
    priv->v = dict_variable(priv->environment, "v");
    priv->w = dict_variable(priv->environment, "w");
    priv->h = dict_variable(priv->environment, "h");
    priv->t = dict_variable(priv->environment, "t");
    priv->d = dict_variable(priv->environment, "d");

    priv->red = dict_variable(priv->environment, "red");
    priv->green = dict_variable(priv->environment, "green");
    priv->blue = dict_variable(priv->environment, "blue");
    priv->linesize = dict_variable(priv->environment, "linesize");
    priv->skip = dict_variable(priv->environment, "skip");
    priv->drawmode = dict_variable(priv->environment, "drawmode");

    /* instantiate variables -- as long as no other variables are included in the environment after these next steps, these pointers will point to valid data. */
    priv->n = dict_variable(priv->environment, "n");
    *priv->n = 0.0;
    priv->b = dict_variable(priv->environment, "b");
    *priv->b = 0.0;
    priv->x = dict_variable(priv->environment, "x");
    *priv->x = 0.0;
    priv->y = dict_variable(priv->environment, "y");
    *priv->y = 0.0;
    priv->i = dict_variable(priv->environment, "i");
    *priv->i = 0.0;
    priv->v = dict_variable(priv->environment, "v");
    *priv->v = 0.0;
    priv->w = dict_variable(priv->environment, "w");
    *priv->w = 0.0;
    priv->h = dict_variable(priv->environment, "h");
    *priv->h = 0.0;
    priv->t = dict_variable(priv->environment, "t");
    *priv->t = 0.0;
    priv->d = dict_variable(priv->environment, "d");
    *priv->d = 0.0;

    priv->red = dict_variable(priv->environment, "red");
    *priv->red = 1;
    priv->green = dict_variable(priv->environment, "green");
    *priv->green = 1;
    priv->blue = dict_variable(priv->environment, "blue");
    *priv->blue = 1;
    priv->linesize = dict_variable(priv->environment, "linesize");
    *priv->linesize = 1;
    priv->skip = dict_variable(priv->environment, "skip");
    *priv->skip = 0;
    priv->drawmode = dict_variable(priv->environment, "drawmode");
    *priv->drawmode = 0;

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

    if(priv->pipeline != NULL)
        visual_object_unref(VISUAL_OBJECT(priv->pipeline));

    int i;
    for(i = 0; i < 4; i++) {
	if(priv->runnable[i] != NULL)
	        expr_free(priv->runnable[i]);
    }

    if(priv->environment != NULL)
        dict_free(priv->environment);

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
    /* instantiate variables -- as long as no other variables are included in the environment after these next steps, these pointers will point to valid data. */
    priv->n = dict_variable(priv->environment, "n");
    priv->b = dict_variable(priv->environment, "b");
    priv->x = dict_variable(priv->environment, "x");
    priv->y = dict_variable(priv->environment, "y");
    priv->i = dict_variable(priv->environment, "i");
    priv->v = dict_variable(priv->environment, "v");
    priv->w = dict_variable(priv->environment, "w");
    priv->h = dict_variable(priv->environment, "h");
    priv->t = dict_variable(priv->environment, "t");
    priv->d = dict_variable(priv->environment, "d");

    priv->red = dict_variable(priv->environment, "red");
    priv->green = dict_variable(priv->environment, "green");
    priv->blue = dict_variable(priv->environment, "blue");
    priv->linesize = dict_variable(priv->environment, "linesize");
    priv->skip = dict_variable(priv->environment, "skip");
    priv->drawmode = dict_variable(priv->environment, "drawmode");
}

static __inline int makeint(double t)
{
  if (t <= 0.0) return 0;
  if (t >= 1.0) return 255;
  return (int)(t*255.0);
}

// We don't use VisAudio directly in pipeline sub-elements normally. LVAVSPipeline provides ->fbout and ->framebuffer. Superscope only uses ->framebuffer. Some elements, such as blur, will write to ->fbout. Internally the two buffers are blended together and swapped out for one another ass appropriate. set pipeline->inverted=1 if you're using ->fbout
int lv_superscope_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{

    SuperScopePrivate *priv = (SuperScopePrivate *)visual_object_get_private (VISUAL_OBJECT (plugin));
    LVAVSPipeline *pipeline = priv->pipeline;
    uint32_t *buf = pipeline->framebuffer;
    int isBeat;

    VisBuffer pcm;
    float pcmbuf[576];

                        
    VisColor color;
    visual_color_from_uint32(&color, 0xffffffff);
    avs_gfx_line_ints(video, 0, 0, video->width, video->height, &color);

    isBeat = priv->pipeline->isBeat;

    if(priv->needs_init) {
        priv->needs_init = FALSE;
        scope_run(priv, SCOPE_RUNNABLE_INIT);
    }

//  visual_video_fill_color(video, visual_color_black()); 
    //buf = (uint32_t *)visual_video_get_pixels (video);

    int a, l, lx = 0, ly = 0, x = 0, y = 0;
    int32_t current_color;
    int ws=(priv->channel_source&4)?1:0;
    int xorv=(ws*128)^128;
    uint16_t fa_data[576];

    
    if((priv->channel_source&3) >= 2)
    {
        for(x = 0; x < 576; x++) {
            pcmbuf[x] = pipeline->audiodata[ws^1][0][x] / 2 + pipeline->audiodata[ws^1][1][x] / 2;
        }
    }
    else
    {
        printf(" < 2\n");
        for(x = 0; x < 576; x++) {
            pcmbuf[x] = pipeline->audiodata[ws^1][priv->channel_source&3][x];
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

    set_vars(priv);
    *priv->h = video->height;
    *priv->w = video->width;
    *priv->b = isBeat?1.0:0.0;
    *priv->blue = (current_color&0xff)/255.0;
    *priv->green = ((current_color>>8)&0xff)/255.0;
    *priv->red = ((current_color>>16)&0xff)/255.0;
    *priv->skip = 0.0;
    *priv->linesize = (double) ((pipeline->blendmode&0xff0000)>>16);
    *priv->drawmode = priv->draw_type ? 1.0 : 0.0;


    scope_run(priv, SCOPE_RUNNABLE_FRAME);
    set_vars(priv);
    if (isBeat) {
	set_vars(priv);
        scope_run(priv, SCOPE_RUNNABLE_BEAT);
	set_vars(priv);
    }

    //int candraw=0;
    l = *priv->n;
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

	set_vars(priv);
        *priv->v = pcmbuf[a * 288 / l];
        *priv->i = a/(double)(l-1);
        *priv->skip = 0.0;
        scope_run(priv, SCOPE_RUNNABLE_POINT);
	set_vars(priv);

        x = (int)((*priv->x + 1.0) * video->width * 0.5);
        y = (int)((*priv->y + 1.0) * video->height * 0.5);

	printf("x = %f, y = %f\n", *priv->x, *priv->y);

        if (*priv->skip >= 0.00001)
            continue;

        int this_color = makeint(*priv->blue) | (makeint(*priv->green) << 8) | (makeint(*priv->red) << 16);

        if (1) {//*priv->drawmode < 0.00001) {
                if (y >= 0 && y < video->height && x >= 0 && x < video->width)
                    BLEND_LINE(buf+x+y*video->width, this_color, (unsigned char**)pipeline->blendtable, (int)pipeline->blendmode);
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


