#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <libvisual/libvisual.h>

#include "actor_gdkpixbuf.h"

static int load_new_file (PixbufPrivate *priv, const char *filename);
static int update_scaled_pixbuf (PixbufPrivate *priv);
static int update_into_visvideo (PixbufPrivate *priv, GdkPixbuf *src);

int act_gdkpixbuf_init (VisPluginData *plugin);
int act_gdkpixbuf_cleanup (VisPluginData *plugin);
int act_gdkpixbuf_requisition (VisPluginData *plugin, int *width, int *height);
int act_gdkpixbuf_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
int act_gdkpixbuf_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *act_gdkpixbuf_palette (VisPluginData *plugin);
int act_gdkpixbuf_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

const VisPluginInfo *get_plugin_info (int *count)
{
	static const VisActorPlugin actor[] = {{
		.requisition = act_gdkpixbuf_requisition,
		.palette = act_gdkpixbuf_palette,
		.render = act_gdkpixbuf_render,
		.depth = VISUAL_VIDEO_DEPTH_24BIT
	}};

	static const VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_ACTOR,

		.plugname = "gdkpixbuf",
		.name = "GdkPixbuf image loader",
		.author = "Dennis Smit <ds@nerds-incorporated.org>",
		.version = "0.0.1",
		.about = "The GdkPixbuf image loader for libvisual",
		.help = "This plugin can be used to show images",

		.init = act_gdkpixbuf_init,
		.cleanup = act_gdkpixbuf_cleanup,
		.events = act_gdkpixbuf_events,

		.plugin = (void *) &actor[0]
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int act_gdkpixbuf_init (VisPluginData *plugin)
{
	PixbufPrivate *priv;
	VisParamContainer *paramcontainer = visual_plugin_get_params (plugin);;
	
	static const VisParamEntry params[] = {
		VISUAL_PARAM_LIST_ENTRY_STRING	("filename",	""),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("scaled",	TRUE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("aspect",	FALSE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("center",	TRUE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("set size",	FALSE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("width",	0),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("height",	0),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("x",		0),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("y",		0),
		VISUAL_PARAM_LIST_ENTRY_INTEGER	("interpolate",	0),
		VISUAL_PARAM_LIST_END
	};

	priv = visual_mem_new0 (PixbufPrivate, 1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	/* Initialize g_type, needed for GdkPixbuf */
	g_type_init ();
	
	visual_param_container_add_many (paramcontainer, params);
	
	return 0;
}

int act_gdkpixbuf_cleanup (VisPluginData *plugin)
{
	PixbufPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	if (priv->filename != NULL)
		free (priv->filename);

	if (priv->pixbuf != NULL)
		g_object_unref (priv->pixbuf);

	if (priv->scaled != NULL)
		g_object_unref (priv->scaled);

	if (&priv->target.pixels != NULL)
		visual_video_free_buffer (&priv->target);

	visual_mem_free (priv);

	return 0;
}

int act_gdkpixbuf_requisition (VisPluginData *plugin, int *width, int *height)
{
	int reqw = *width;
	int reqh = *height;

	if (reqw < 1)
		reqw = 1;

	if (reqh < 1)
		reqh = 1;

	*width = reqw;
	*height = reqh;

	return 0;
}

int act_gdkpixbuf_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
	PixbufPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	
	visual_video_set_dimension (video, width, height);

	priv->width = width;
	priv->height = height;

	if (priv->pixbuf != NULL)
		update_scaled_pixbuf (priv);
	else {
		/* If there is no image reset the VisVideo pixels, just to be sure */
		if (priv->target.pixels != NULL)
			visual_video_free_buffer (&priv->target);
		
		visual_video_set_buffer (&priv->target, NULL);
	}
	return 0;
}

int act_gdkpixbuf_events (VisPluginData *plugin, VisEventQueue *events)
{
	PixbufPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	VisParamEntry *param;
	VisEvent ev;

	while (visual_event_queue_poll (events, &ev)) {
		switch (ev.type) {
			case VISUAL_EVENT_RESIZE:
				act_gdkpixbuf_dimension (plugin, ev.resize.video,
						ev.resize.width, ev.resize.height);
				break;

			case VISUAL_EVENT_PARAM:
				param = ev.param.param;

				if (visual_param_entry_is (param, "filename")) {
					visual_log (VISUAL_LOG_DEBUG, "New file to be loaded: %s\n",
							visual_param_entry_get_string (param));

					load_new_file (priv, visual_param_entry_get_string (param));

				} else if (visual_param_entry_is (param, "scaled")) {
					priv->set_scaled = visual_param_entry_get_integer (param);
					
					update_scaled_pixbuf (priv);
					
				} else if (visual_param_entry_is (param, "aspect")) {
					priv->aspect = visual_param_entry_get_integer (param);

					update_scaled_pixbuf (priv);

				} else if (visual_param_entry_is (param, "center")) {
					priv->center = visual_param_entry_get_integer (param);
				
				} else if (visual_param_entry_is (param, "set size")) {
					priv->set_size = visual_param_entry_get_integer (param);

					update_scaled_pixbuf (priv);

				} else if (visual_param_entry_is (param, "width")) {
					priv->set_width = visual_param_entry_get_integer (param);

					update_scaled_pixbuf (priv);
				
				} else if (visual_param_entry_is (param, "height")) {
					priv->set_height = visual_param_entry_get_integer (param);

					update_scaled_pixbuf (priv);
				
				} else if (visual_param_entry_is (param, "x")) {
					priv->x_offset = visual_param_entry_get_integer (param);
				
				} else if (visual_param_entry_is (param, "y")) {
					priv->y_offset = visual_param_entry_get_integer (param);
				
				} else if (visual_param_entry_is (param, "interpolate")) {
					priv->interpolate = visual_param_entry_get_integer (param);

					update_scaled_pixbuf (priv);

				}
					
			default: /* to avoid warnings */
				break;
		}
	}

	return 0;
}

VisPalette *act_gdkpixbuf_palette (VisPluginData *plugin)
{
	return NULL;
}

int act_gdkpixbuf_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
	PixbufPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));
	
	if (priv->target.pixels != NULL) {
		if (priv->center == TRUE) {
			int xoff, yoff;
			
			xoff = (video->width - priv->target.width) / 2;
			yoff = (video->height - priv->target.height) / 2;

			visual_video_blit_overlay (video, &priv->target, xoff, yoff, FALSE);

		} else {
			visual_video_blit_overlay (video, &priv->target, priv->x_offset, priv->y_offset, FALSE);
		}
	}

	return 0;
}

static int load_new_file (PixbufPrivate *priv, const char *filename)
{
	if (priv->pixbuf != NULL)
		g_object_unref (priv->pixbuf);

	if (priv->scaled != NULL)
		g_object_unref (priv->scaled);

	if (priv->filename != NULL)
		free (priv->filename);
	
	priv->filename = strdup (filename);

	priv->pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
	visual_log_return_val_if_fail (priv->pixbuf != NULL, -1);	

	return update_scaled_pixbuf (priv);
}

static int update_scaled_pixbuf (PixbufPrivate *priv)
{
	GdkInterpType inter;

	if (priv->scaled != NULL)
		g_object_unref (priv->scaled);

	switch (priv->interpolate) {
		case 0:
			inter = GDK_INTERP_NEAREST;
			break;

		case 1:
			inter = GDK_INTERP_TILES;
			break;

		case 2:
			inter = GDK_INTERP_BILINEAR;
			break;

		case 3:
			inter = GDK_INTERP_HYPER;
			break;

		default:
			inter = GDK_INTERP_NEAREST;
			break;
	}
	
	if (priv->set_scaled == TRUE) {
		if (priv->set_size == TRUE) {
			/* We want to allow this, but gdk_pixbuf does spit warnings, so we catch this */
			if (priv->set_width == 0 || priv->set_height == 0) {
				visual_video_set_buffer (&priv->target, NULL);

				return 0;
			}

			priv->scaled = gdk_pixbuf_scale_simple (priv->pixbuf, priv->set_width, priv->set_height, inter);

		} else if (priv->aspect == TRUE) {
			int as_w, as_h;
			int rw, rh;

			rw = gdk_pixbuf_get_width (priv->pixbuf);
			rh = gdk_pixbuf_get_height (priv->pixbuf);

			/* Determine which dimension we need to create aspect */
			if ((priv->width - rw) > (priv->height - rh)) {
				as_h = priv->height;
				as_w = rw * ((float) as_h / rh);
			} else {
				as_w = priv->width;
				as_h = rh * ((float) as_w / rw);
			}
			
			priv->scaled = gdk_pixbuf_scale_simple (priv->pixbuf, as_w, as_h, inter);
			
		} else {
			priv->scaled = gdk_pixbuf_scale_simple (priv->pixbuf, priv->width, priv->height, inter);
		}
		
		visual_log_return_val_if_fail (priv->scaled != NULL, -1);

		update_into_visvideo (priv, priv->scaled);

	} else {
		visual_log_return_val_if_fail (priv->pixbuf != NULL, -1);

		update_into_visvideo (priv, priv->pixbuf);

	}
	
	return 0;
}

static int update_into_visvideo (PixbufPrivate *priv, GdkPixbuf *src)
{
	VisVideo *target;
	VisVideo bgr;
	
	target = &priv->target;

	/* Create a VisVideo from the pixbuf */
	visual_video_set_depth (&bgr,
			visual_video_depth_enum_from_value (gdk_pixbuf_get_n_channels (src) * 8));
	visual_video_set_dimension (&bgr, gdk_pixbuf_get_width (src), gdk_pixbuf_get_height (src));
	visual_video_set_pitch (&bgr, gdk_pixbuf_get_rowstride (src));
	visual_video_set_buffer (&bgr, gdk_pixbuf_get_pixels (src));

	if (target->pixels != NULL)
		visual_video_free_buffer (target);

	visual_video_clone (target, &bgr);
	visual_video_allocate_buffer (target);

	/* Gdk uses a different color order than we do */
	visual_video_color_bgr_to_rgb (target, &bgr);

	return 0;
}

