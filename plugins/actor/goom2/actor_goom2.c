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

		.plugin = (void *) &actor[0]
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int lv_goom_init (VisPluginData *plugin)
{
	GoomPrivate *priv;

	priv = visual_mem_new0 (GoomPrivate, 1);
	plugin->priv = priv;

	priv->goominfo = goom_init (128, 128);
	
	return 0;
}

int lv_goom_cleanup (VisPluginData *plugin)
{
	GoomPrivate *priv = plugin->priv;

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
	GoomPrivate *priv = plugin->priv;

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
	GoomPrivate *priv = plugin->priv;
	VisSongInfo *songinfo = plugin->songinfo;
	VisParamContainer *paramcontainer;
	VisParamEntry *param;
	short pcmdata[2][512];
	uint32_t *buf;
	int showinfo = TRUE;

	memcpy (pcmdata, audio->pcm, sizeof (short) * 512 * 2);

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

	memcpy (video->screenbuffer, buf, video->width * video->height * video->bpp);

	return 0;
}
