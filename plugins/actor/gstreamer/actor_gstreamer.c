/* Libvisual-plugins - Standard plugins for libvisual
 * 
 * Copyright (C) 2004, 2005 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <gettext.h>

#include <libvisual/libvisual.h>

#include <gst/gst.h>


#define BARS 16

typedef struct {
	VisVideo *old_video;
	GstPipeline *pipe;
} GstreamerPrivate;


static void have_data (GstElement *sink, GstBuffer *buffer, gpointer data);

int act_gstreamer_init (VisPluginData *plugin);
int act_gstreamer_cleanup (VisPluginData *plugin);
int act_gstreamer_requisition (VisPluginData *plugin, int *width, int *height);
int act_gstreamer_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
int act_gstreamer_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *act_gstreamer_palette (VisPluginData *plugin);
int act_gstreamer_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

const VisPluginInfo *get_plugin_info (int *count)
{
	static VisActorPlugin actor[] = {{
		.requisition = act_gstreamer_requisition,
		.palette = act_gstreamer_palette,
		.render = act_gstreamer_render,
		.depth = VISUAL_VIDEO_DEPTH_24BIT
	}};

	static VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_ACTOR,

		.plugname = "gstreamer",
		.name = "libvisual gstreamer",
		.author = N_("Dennis Smit  <synap@yourbase.nl>"),
		.version = "0.1",
		.about = N_("Libvisual gstreamer plugin"),
		.help = N_("Capable of using gstreamer to play video, or do effects using the gstreamer pipeline"),
		.license = VISUAL_PLUGIN_LICENSE_LGPL,

		.init = act_gstreamer_init,
		.cleanup = act_gstreamer_cleanup,
		.events = act_gstreamer_events,

		.plugin = VISUAL_OBJECT (&actor[0])
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int act_gstreamer_init (VisPluginData *plugin)
{
	GstreamerPrivate *priv;

#if ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
#endif

	priv = visual_mem_new0 (GstreamerPrivate, 1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	return 0;
}

int act_gstreamer_cleanup (VisPluginData *plugin)
{
	GstreamerPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	visual_mem_free (priv);

	return 0;
}

int act_gstreamer_requisition (VisPluginData *plugin, int *width, int *height)
{

	return 0;
}

int act_gstreamer_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
	visual_video_set_dimension (video, width, height);

	return 0;
}

int act_gstreamer_events (VisPluginData *plugin, VisEventQueue *events)
{
	VisEvent ev;

	while (visual_event_queue_poll (events, &ev)) {
		switch (ev.type) {
			case VISUAL_EVENT_RESIZE:
				act_gstreamer_dimension (plugin, ev.event.resize.video,
						ev.event.resize.width, ev.event.resize.height);
				break;


			default:
				break;
		}
	}

	return 0;
}

VisPalette *act_gstreamer_palette (VisPluginData *plugin)
{
	return NULL;
}

int act_gstreamer_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
	GstreamerPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	static int playing = 0;

	if (playing == 0) {
		char pipe[1024];

		gst_init (NULL, NULL);
/*
		snprintf(pipe, 1024, "filesrc location=%s ! decodebin ! ffmpegcolorspace ! "
				"videoscale ! video/x-raw-rgb,bpp=32,depth=32,width=%d,height=%d,"
				"red_mask=0xff000000,green_mask=0x00ff0000,blue_mask=0x0000ff00 !"
				"fakesink name=sink sync=true", "test.mpg", video->width, video->height);

*/
		snprintf(pipe, 1024, "filesrc location=%s ! decodebin ! ffmpegcolorspace ! "
				"video/x-raw-rgb,bpp=24,depth=24 ! "
				"fakesink name=sink signal-handoffs=true", "test.mpg");

		GError *err = NULL;

		priv->pipe = gst_parse_launch (pipe, &err);

		if (err)
			g_print ("FUCK UUU %s\n", err->message);

		printf ("ja hooradfh\n");

		gst_element_set_state (GST_ELEMENT (priv->pipe), GST_STATE_PLAYING);

		g_signal_connect (gst_bin_get_by_name_recurse_up (GST_BIN (priv->pipe), "sink"),
				"handoff", G_CALLBACK (have_data), video);

		playing = 1;
	}

//	g_signal_handlers_disconnect_by_func (gst_bin_get_by_name_recurse_up (GST_BIN (priv->pipe), "sink"),
//			G_CALLBACK (have_data), priv->old_video);


	gst_bin_iterate (GST_BIN (priv->pipe));

	priv->old_video = video;

	return 0;
}

static void have_data (GstElement *sink, GstBuffer *buffer, gpointer data)
{
	VisVideo *video = data;
	uint32_t *dest = visual_video_get_pixels (video);
	uint32_t *src = (uint32_t *) GST_BUFFER_DATA (buffer);

	printf ("ja hoor\n");

	visual_mem_copy (dest, src, GST_BUFFER_SIZE (buffer));
}

