/* Libvisual-plugins - Standard plugins for libvisual
 * 
 * Copyright (C) 2004, 2005 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *          Pascal Brochart <pbrochart@tuxfamily.org> and many others
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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <gettext.h>

#include "nebulus.h"
#include "config.h"


typedef struct {

	VisTime rendertime;

} NebulusPrivate;


float framerate = 50;
GLfloat scale, heights[16][16];

int loudness = 0, too_long;
int beat = 0, beat_compteur = 0;
static int beathistory[BEAT_MAX];
static int beatbase;

short pcm_data[512];
nebulus general = { 32, 32, 0, 4, 4, 100, 100, 0, FALSE, FALSE,
	                    TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
nebulus *point_general = &general;

effect my_effect[EFFECT_NUMBER] = {{ "Knot", 100 }, { "Spectrum", 100 },
	{ "Face blur", 100 }, { "Glthreads", 100 },
	{ "Tunnel", 100 }, { "Tentacles", 100 },
	{ "Twist", 100 }, { "Child", 100 },
	{ "Energy", 100 }};
effect my_effect_old[EFFECT_NUMBER];

GLint maxtexsize;

int lv_nebulus_init (VisPluginData *plugin);
int lv_nebulus_cleanup (VisPluginData *plugin);
int lv_nebulus_requisition (VisPluginData *plugin, int *width, int *height);
int lv_nebulus_events (VisPluginData *plugin, VisEventQueue *events);
VisPalette *lv_nebulus_palette (VisPluginData *plugin);
int lv_nebulus_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio);

static int lv_nebulus_dimension (VisPluginData *plugin, VisVideo *video, int width, int height);
static int nebulus_random_effect ();
static int nebulus_detect_beat (int loudness);
static int nebulus_sound (NebulusPrivate *priv, VisAudio *audio);
static int nebulus_draw (NebulusPrivate *priv, VisVideo *video);

/* Main plugin stuff */
const VisPluginInfo *get_plugin_info (int *count)
{
	static const VisActorPlugin actor[] = {{
		.requisition = lv_nebulus_requisition,
		.palette = lv_nebulus_palette,
		.render = lv_nebulus_render,
		.depth = VISUAL_VIDEO_DEPTH_GL
	}};

	static const VisPluginInfo info[] = {{
		.struct_size = sizeof (VisPluginInfo),
		.api_version = VISUAL_PLUGIN_API_VERSION,
		.type = VISUAL_PLUGIN_TYPE_ACTOR,

		.plugname = "nebulus",
		.name = PACKAGE_NAME,
		.author = N_("Original by: Pascal Brochart <pbrochart@tuxfamily.org> and many others, Port and maintaince by: Dennis Smit <ds@nerds-incorporated.org>"),
		.version = VERSION,
		.about = N_("Libvisual nebulus plugin"),
		.help = N_("This plugin shows multiple visual effect using openGL"),

		.init = lv_nebulus_init,
		.cleanup = lv_nebulus_cleanup,
		.events = lv_nebulus_events,

		.plugin = VISUAL_OBJECT (&actor[0])
	}};

	*count = sizeof (info) / sizeof (*info);

	return info;
}

int lv_nebulus_init (VisPluginData *plugin)
{
	NebulusPrivate *priv;

#if ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
#endif

	visual_log_return_val_if_fail (plugin != NULL, -1);

	priv = visual_mem_new0 (NebulusPrivate, 1);
	visual_object_set_private (VISUAL_OBJECT (plugin), priv);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxtexsize);
	if (maxtexsize < 256) {
		visual_log (VISUAL_LOG_CRITICAL, _("Nebulus: max texture size is lower than 256"));
		return -1;
	}

	if (tunnel_first)
		precalculate_tunnel();

	init_gl();

	return 0;
}

int lv_nebulus_cleanup (VisPluginData *plugin)
{
	NebulusPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	visual_log_return_val_if_fail (plugin != NULL, -1);

	if (!face_first)
		glDeleteLists(facedl, 1);
	if (!tentacles_first)
		glDeleteLists(cubedl, 1);
	if (!child_first)
		glDeleteLists(childdl, 1);

	delete_gl_texture(knotbg);
	delete_gl_texture(tunnel);
	delete_gl_texture(tentacle);
	delete_gl_texture(twist);
	delete_gl_texture(twistbg);
	delete_gl_texture(texchild);
	delete_gl_texture(childbg);
	delete_gl_texture(energy);

	visual_video_free_buffer (&child_image);
	visual_video_free_buffer (&energy_image);
	visual_video_free_buffer (&tentacle_image);
	visual_video_free_buffer (&tunnel_image);
	visual_video_free_buffer (&twist_image);
	visual_video_free_buffer (&background_image);

	visual_mem_free (priv);

	return 0;
}

int lv_nebulus_requisition (VisPluginData *plugin, int *width, int *height)
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

VisPalette *lv_nebulus_palette (VisPluginData *plugin)
{
	return NULL;
}

static int lv_nebulus_dimension (VisPluginData *plugin, VisVideo *video, int width, int height)
{
	visual_log_return_val_if_fail (video != NULL, -1);

	visual_video_set_dimension (video, width, height);

	glViewport (0, 0, width, height);

	point_general->WIDTH = width;
	point_general->HEIGHT = height;
	
	return 0;
}

int lv_nebulus_events (VisPluginData *plugin, VisEventQueue *events)
{
	VisEvent ev;

	while (visual_event_queue_poll (events, &ev)) {
		switch (ev.type) {
			case VISUAL_EVENT_RESIZE:
				lv_nebulus_dimension (plugin, ev.resize.video,
						ev.resize.width, ev.resize.height);
				break;
			default:
				break;
		}
	}

	return 0;
}

int lv_nebulus_render (VisPluginData *plugin, VisVideo *video, VisAudio *audio)
{
	NebulusPrivate *priv = visual_object_get_private (VISUAL_OBJECT (plugin));

	visual_log_return_val_if_fail (plugin != NULL, -1);
	visual_log_return_val_if_fail (video != NULL, -1);
	visual_log_return_val_if_fail (audio != NULL, -1);

	nebulus_sound (priv, audio);
	nebulus_draw (priv, video);

	return 0;
}

static int nebulus_calc_fps (NebulusPrivate *priv)
{
	// FIXME make
	framerate = 25;
	return 40;
}

static int nebulus_random_effect ()
{
	int i;
	int rnd_effect = 0, new_effect = 0, effect = 0;

	for (i = 0; i < EFFECT_NUMBER; i++)
		rnd_effect += my_effect[i].value * 100;     /* 100 pour + de precision */
	if (!rnd_effect)
		return point_general->effect;
	new_effect = rand() % rnd_effect;
	rnd_effect = 0;
	for (i = 0; i < EFFECT_NUMBER; i++) {
		rnd_effect += my_effect[i].value * 100;     /* 100 pour + de precision */
		if (new_effect <= rnd_effect) {
			effect = i;
			if (!effect)
				effect = EFFECT_NUMBER;
			return effect;
		}
	}
	return point_general->effect;
}

static int nebulus_detect_beat (int loudness)
{
	static int    aged;
	static int    lowest;
	static int elapsed;
	static int isquiet;
	static int prevbeat;
	int i, j;
	int total;
	int sensitivity;
	int detected_beat;

	aged = (aged * 7 + loudness) >> 3;
	elapsed++;

	if (aged < 2000 || elapsed > BEAT_MAX) {
		elapsed = 0;
		lowest = aged;
		memset(beathistory, 0, sizeof beathistory);
	}
	else if (aged < lowest)
		lowest = aged;

	j = (beatbase + elapsed) % BEAT_MAX;
	beathistory[j] = loudness - aged;
	detected_beat = FALSE;
	if (elapsed > 15 && aged > 2000 && loudness * 4 > aged * 5) {
		for (i = BEAT_MAX / elapsed, total = 0;
				--i > 0;
				j = (j + BEAT_MAX - elapsed) % BEAT_MAX)
		{
			total += beathistory[j];
		}
		total = total * elapsed / BEAT_MAX;
		sensitivity = 6;
		i = 3 - abs(elapsed - prevbeat)/2;
		if (i > 0)
			sensitivity += i;
		if (total * sensitivity > aged) {
			prevbeat = elapsed;
			beatbase = (beatbase + elapsed) % BEAT_MAX;
			lowest = aged;
			elapsed = 0;
			detected_beat = TRUE;
		}
	}
	if (aged < (isquiet ? 1500 : 500))
		isquiet = TRUE;
	else
		isquiet = FALSE;

	return detected_beat;
}

static int nebulus_sound (NebulusPrivate *priv, VisAudio *audio)
{
	int i, c, y, tmp;
	GLfloat val, energy = 0;
	int xscale[] = { 0, 1, 2, 3, 5, 7, 10, 14, 20, 28, 40, 54, 74, 101, 137, 187, 255 };

	/* Copy over the pcmdata into the internal pcmdata */
	memcpy (pcm_data, audio->pcm, sizeof(pcm_data));

	for(y = 15; y > 0; y--) {
		for(i = 0; i < 16; i++)
			heights[y][i] = heights[y - 1][i];
	}
	for(i = 0; i < NUM_BANDS; i++) {
		for(c = xscale[i], y = 0; c < xscale[i + 1]; c++) {
			if(audio->freq[0][c] > y)
				y = audio->freq[0][c];
		}
		loudness += (y / (xscale[i + 1] - xscale[i] + 1)) *
			(abs (i - NUM_BANDS / 2) + NUM_BANDS / 2) * (4 + i);
		y >>= 7;
		if(y > 0)
			val = (log(y) * scale);
		else
			val = 0;
		heights[0][i] = val;
	}
	loudness /= (NUM_BANDS * 4);
	beat = nebulus_detect_beat (loudness);
	too_long++;
	if (!point_general->freeze) {
		if (too_long > 1000) {
			too_long = 0;
			point_general->effect = nebulus_random_effect();
			point_general->changement = TRUE;
		}
		if (beat) {
			if (beat_compteur > point_general->beat - 1) {
				point_general->effect = nebulus_random_effect();
				point_general->changement = TRUE;
				beat_compteur = 0;
				too_long = 0;
			}
			beat_compteur += beat;
		}
	}
	for (i = 0; i < 256; i++) {
		tmp = audio->freq[0][i] >> 4;
		energy += tmp * tmp;
	}
	energy =  energy / 65536 / 256 * 256;
	energy += 6.0f;
	energy *= 1.2f;
	if (energy > 10)
		energy = 10;
	point_general->energy = energy;

	return 0;
}

static int nebulus_draw (NebulusPrivate *priv, VisVideo *video)
{
	draw_scene ();
	glFinish ();

	nebulus_calc_fps (priv);

	return 0;
}

