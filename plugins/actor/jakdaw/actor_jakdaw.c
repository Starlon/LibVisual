/* Libvisual-plugins - Standard plugins for libvisual
 * 
 * Copyright (C) 2004, 2005 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id:
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "feedback.h"
#include "plotter.h"

#include <libvisual/libvisual.h>

int act_jakdaw_init (VisPluginData *plugin);
int act_jakdaw_cleanup (VisPluginData *plugin);
int act_jakdaw_requisition (VisPluginData *plugin, int *width, int *height);
int act_jakdaw_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
int act_jakdaw_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *act_jakdaw_palette (VisPluginData *plugin);
int act_jakdaw_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

const VisPluginInfo *get_plugin_info (int *count)
{
	static const VisActorPlugin actor[] = {{
		.requisition = act_jakdaw_requisition,
		.palette = act_jakdaw_palette,
		.render = act_jakdaw_render,
		.depth = VISUAL_VIDEO_DEPTH_32BIT
	}};

	static const VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_ACTOR,

		.plugname = "jakdaw",
		.name = "Jakdaw plugin",
		.author = "Original by: Christopher Wilson <Jakdaw@usa.net>, Port by: Dennis Smit <ds@nerds-incorporated.org>",
		.version = "0.0.1",
		.about = "The jakdaw visual plugin",
		.help = "This is the libvisual port of the xmms Jakdaw plugin",

		.init = act_jakdaw_init,
		.cleanup = act_jakdaw_cleanup,
		.events = act_jakdaw_events,

		.plugin = VISUAL_OBJECT (&actor[0])
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int act_jakdaw_init (VisPluginData *plugin)
{
	JakdawPrivate *priv;
	VisParamContainer *paramcontainer = visual_plugin_get_params (plugin);

	static const VisParamEntry params[] = {
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("zoom mode",		FEEDBACK_ZOOMRIPPLE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("plotter trigger",	PLOTTER_COLOUR_MUSICTRIG),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("plotter type",	PLOTTER_SCOPE_LINES),
		VISUAL_PARAM_LIST_END
	};

	static const VisParamEntry zoomparamchoices[] = {
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Zoom ripple",		FEEDBACK_ZOOMRIPPLE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Blur only",		FEEDBACK_BLURONLY),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Zoom rotate",		FEEDBACK_ZOOMROTATE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Scroll",		FEEDBACK_SCROLL),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Into screen",		FEEDBACK_INTOSCREEN),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Ripple",		FEEDBACK_NEWRIPPLE),
		VISUAL_PARAM_LIST_END
	};

	static const VisParamEntry colorparamchoices[] = {
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Solid",		PLOTTER_COLOUR_SOLID),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Random",		PLOTTER_COLOUR_RANDOM),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("On music",		PLOTTER_COLOUR_MUSICTRIG),
		VISUAL_PARAM_LIST_END
	};
	
		
	static const VisParamEntry scopeparamchoices[] = {
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Lines",		PLOTTER_SCOPE_LINES),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Dots",		PLOTTER_SCOPE_DOTS),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Solid",		PLOTTER_SCOPE_SOLID),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("Nothing",		PLOTTER_SCOPE_NOTHING),
		VISUAL_PARAM_LIST_END
	};

	/* UI vars */
	VisUIWidget *table;
	VisUIWidget *label1;
	VisUIWidget *label2;
	VisUIWidget *label3;
	VisUIWidget *popup1;
	VisUIWidget *popup2;
	VisUIWidget *popup3;

	priv = visual_mem_new0 (JakdawPrivate, 1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	priv->rcontext = visual_plugin_get_random_context (plugin);

	priv->decay_rate = 1;
	
	priv->zoom_ripplesize = 32;
	priv->zoom_ripplefact = 0.1;
	priv->zoom_zoomfact = 0.9;

	priv->plotter_amplitude = 100;

	/* FIXME make param of this one as well */
	priv->plotter_scopecolor = 0xff00ff;

	visual_param_container_add_many (paramcontainer, params);

	table = visual_ui_table_new (3, 2);
	
	label1 = visual_ui_label_new ("Blur mode:", FALSE);
	label2 = visual_ui_label_new ("Plotter color:", FALSE);
	label3 = visual_ui_label_new ("Plotter type:", FALSE);

	popup1 = visual_ui_popup_new ();
	visual_ui_widget_set_tooltip (popup1, "The method of blurring");
	visual_ui_mutator_set_param (VISUAL_UI_MUTATOR (popup1), visual_param_container_get (paramcontainer, "zoom mode"));
	visual_ui_choice_add_many (VISUAL_UI_CHOICE (popup1), zoomparamchoices);

	popup2 = visual_ui_popup_new ();
	visual_ui_widget_set_tooltip (popup2, "The color of the plotter");
	visual_ui_mutator_set_param (VISUAL_UI_MUTATOR (popup2), visual_param_container_get (paramcontainer, "plotter trigger"));
	visual_ui_choice_add_many (VISUAL_UI_CHOICE (popup2), colorparamchoices);

	popup3 = visual_ui_popup_new ();
	visual_ui_widget_set_tooltip (popup3, "The plotter it's shape");
	visual_ui_mutator_set_param (VISUAL_UI_MUTATOR (popup3), visual_param_container_get (paramcontainer, "plotter type"));
	visual_ui_choice_add_many (VISUAL_UI_CHOICE (popup3), scopeparamchoices);
	
	visual_ui_table_attach (VISUAL_UI_TABLE (table), label1, 0, 0);
	visual_ui_table_attach (VISUAL_UI_TABLE (table), popup1, 0, 1);
	
	visual_ui_table_attach (VISUAL_UI_TABLE (table), label2, 1, 0);
	visual_ui_table_attach (VISUAL_UI_TABLE (table), popup2, 1, 1);

	visual_ui_table_attach (VISUAL_UI_TABLE (table), label3, 2, 0);
	visual_ui_table_attach (VISUAL_UI_TABLE (table), popup3, 2, 1);

	visual_plugin_set_userinterface (plugin, table);

	return 0;
}

int act_jakdaw_cleanup (VisPluginData *plugin)
{
	JakdawPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	VisUIWidget *ui;

	ui = visual_plugin_get_userinterface (plugin);
	visual_object_unref (VISUAL_OBJECT (ui));

	visual_mem_free (priv);

	return 0;
}

int act_jakdaw_requisition (VisPluginData *plugin, int *width, int *height)
{
	int reqw, reqh;

	reqw = *width;
	reqh = *height;

	if (reqw < 32)
		reqw = 32;

	if (reqh < 32)
		reqh = 32;

	*width = reqw;
	*height = reqh;

	return 0;
}

int act_jakdaw_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
	JakdawPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	
	visual_video_set_dimension (video, width, height);

	priv->xres = width;
	priv->yres = height;

	_jakdaw_feedback_reset (priv, width, height);
	_jakdaw_plotter_reset (priv, width, height);
	
	return 0;
}

int act_jakdaw_events (VisPluginData *plugin, VisEventQueue *events)
{
	JakdawPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	VisEvent ev;
	VisParamEntry *param;

	while (visual_event_queue_poll (events, &ev)) {
		switch (ev.type) {
			case VISUAL_EVENT_RESIZE:
				act_jakdaw_dimension (plugin, ev.resize.video,
						ev.resize.width, ev.resize.height);
				break;

			case VISUAL_EVENT_PARAM:
				param = ev.param.param;

				visual_log (VISUAL_LOG_DEBUG, "Param changed: %s\n", param->name);

				if (visual_param_entry_is (param, "zoom mode")) {
					visual_log (VISUAL_LOG_DEBUG, "New value for the zoom mode param: %d\n",
							param->numeric.integer);

					priv->zoom_mode = visual_param_entry_get_integer (param);

					_jakdaw_feedback_reset (priv, priv->xres, priv->yres);
				}
				else if (visual_param_entry_is (param, "plotter trigger")) {
					visual_log (VISUAL_LOG_DEBUG, "New value for the plotter trigger param: %d\n",
							param->numeric.integer);

					priv->plotter_colortype = visual_param_entry_get_integer (param);

					_jakdaw_plotter_reset (priv, priv->xres, priv->yres);
				}
				else if (visual_param_entry_is (param, "plotter type")) {
					visual_log (VISUAL_LOG_DEBUG, "New value for the plotter type param: %d\n",
							param->numeric.integer);

					priv->plotter_scopetype = visual_param_entry_get_integer (param);

					_jakdaw_feedback_reset (priv, priv->xres, priv->yres);
				}
				
				break;

			default: /* to avoid warnings */
				break;
		}
	}

	return 0;
}

VisPalette *act_jakdaw_palette (VisPluginData *plugin)
{
	return NULL;
}

int act_jakdaw_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
	JakdawPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	uint32_t *vscr = video->pixels;

	_jakdaw_feedback_render (priv, vscr);
	_jakdaw_plotter_draw (priv, audio->pcm, audio->freq, vscr);
	
	return 0;
}
