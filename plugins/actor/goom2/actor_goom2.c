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

#include <time.h>
#include <sys/time.h>

#include <goom/goom.h>

#include <libvisual/libvisual.h>

typedef struct {
	PluginInfo	*goominfo; /* The goom internal private struct */
} GoomPrivate;

int lv_goom_init (VisPluginData *plugin);
int lv_goom_cleanup (VisPluginData *plugin);
int lv_goom_requisition (VisPluginData *plugin, int *width, int *height);
int lv_goom_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
int lv_goom_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *lv_goom_palette (VisPluginData *plugin);
int lv_goom_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

const VisPluginInfo *get_plugin_info (int *count)
{
	static const VisActorPlugin actor[] = {{
		.requisition = lv_goom_requisition,
		.palette = lv_goom_palette,
		.render = lv_goom_render,
		.depth = VISUAL_VIDEO_DEPTH_32BIT
	}};

	static const VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_ACTOR,

		.plugname = "goom",
		.name = "libvisual goom2 plugin",
		.author = "Dennis Smit <ds@nerds-incorporated.org>, Goom by: Jean-Christophe Hoelt <jeko@ios-software.com>",
		.version = "0.1",
		.about = "The Libvisual goom2 plugin",
		.help = "This plugin adds support for the supercool goom2 plugin that is simply awesome",

		.init = lv_goom_init,
		.cleanup = lv_goom_cleanup,
		.events = lv_goom_events,

		.plugin = VISUAL_OBJECT (&actor[0])
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int lv_goom_init (VisPluginData *plugin)
{
	GoomPrivate *priv;

	priv = visual_mem_new0 (GoomPrivate, 1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	priv->goominfo = goom_init (128, 128);
	
	return 0;
}

int lv_goom_cleanup (VisPluginData *plugin)
{
	GoomPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	if (priv->goominfo != NULL)
		goom_close (priv->goominfo);

	visual_mem_free (priv);

	return 0;
}

int lv_goom_requisition (VisPluginData *plugin, int *width, int *height)
{
	/* We don't change the value, we can handle anything */	

	return 0;
}

int lv_goom_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
	GoomPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	visual_video_set_dimension (video, width, height);

	goom_set_resolution (priv->goominfo, width, height);

	return 0;
}

int lv_goom_events (VisPluginData *plugin, VisEventQueue *events)
{
	VisEvent ev;

	while (visual_event_queue_poll (events, &ev)) {
		switch (ev.type) {
			case VISUAL_EVENT_RESIZE:
				lv_goom_dimension (plugin, ev.resize.video,
						ev.resize.width, ev.resize.height);

				break;

			default: /* to avoid warnings */
				break;
		}
	}

	return 0;
}

VisPalette *lv_goom_palette (VisPluginData *plugin)
{
	return NULL;
}

int lv_goom_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
	GoomPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	VisSongInfo *songinfo;
	VisParamContainer *paramcontainer;
	VisParamEntry *param;
	short pcmdata[2][512];
	uint32_t *buf;
	int showinfo = TRUE;

	memcpy (pcmdata, audio->pcm, sizeof (short) * 512 * 2);

	/* Retrieve the songinfo */
	songinfo = visual_plugin_actor_get_songinfo (
			VISUAL_PLUGIN_ACTOR (visual_plugin_get_specific (plugin)));

	/* Check the global parameter for showing songinfo in plugins */
	paramcontainer = visual_get_params ();
	param = visual_param_container_get (paramcontainer, "songinfo in plugin");
	if (param != NULL)
		showinfo = visual_param_entry_get_integer (param);
	
	/* FIXME goom should support setting a pointer, so we don't need that final memcpy */
	if (songinfo != NULL && visual_songinfo_age (songinfo) <= 1 && showinfo == TRUE) {
		if (songinfo->type == VISUAL_SONGINFO_TYPE_SIMPLE)
			buf = goom_update (priv->goominfo, pcmdata, 0, 0, songinfo->songname, NULL);
		else if (songinfo->type == VISUAL_SONGINFO_TYPE_ADVANCED)
			buf = goom_update (priv->goominfo, pcmdata, 0, 0, songinfo->song, NULL);
		else
			buf = goom_update (priv->goominfo, pcmdata, 0, 0, NULL, NULL);
	}
	else
		buf = goom_update (priv->goominfo, pcmdata, 0, 0, NULL, NULL);

	memcpy (video->pixels, buf, video->width * video->height * video->bpp);

	return 0;
}
